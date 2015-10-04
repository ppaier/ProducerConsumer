#pragma once

#include "MultiThreadFifo.h"
#include "Processor.h"

#include <vector>

class Worker
{
public:

	Worker(){}
	virtual ~Worker(){}

	virtual bool doWork() = 0;
};

template <typename T>
class Producer : public Worker
{
public:
	Producer() = delete;
	Producer(const Producer &other) = delete;
	Producer(std::shared_ptr<ProducerProcessor<T>> processor, const std::vector<std::shared_ptr<MultiThreadFifo<T>>> & fifosOut) : Worker(),
		m_processor(processor), m_fifosOut(fifosOut) {}
	virtual ~Producer(){}

	virtual bool doWork()
	{
		m_processor->run();
		T dataOut = m_processor->getOutput();
		for (auto & f : m_fifosOut)
		{
			f->push(dataOut);
		}
		return false;
	}

private:
	std::shared_ptr<ProducerProcessor<T>> m_processor;
	std::vector<std::shared_ptr<MultiThreadFifo<T>>> m_fifosOut;
};


template <typename T>
class Consumer : public Worker
{
public:
	Consumer() = delete;
	Consumer(const Consumer &other) = delete;
	Consumer(std::shared_ptr<ConsumerProcessor<T>> processor) : Worker(),
		m_processor(processor) 
	{
		m_fifoIn = std::make_shared<MultiThreadFifo<T>>();
	}
	virtual ~Consumer(){}

	virtual bool doWork()
	{
		if (!m_fifoIn->empty())
		{
			T dataIn = m_fifoIn->pop();
			m_processor->setInput(dataIn);
			m_processor->run();
			return false;
		}
		return true;
	}

	std::shared_ptr<MultiThreadFifo<T>> getFifoIn() { return m_fifoIn; }

private:
	std::shared_ptr<ConsumerProcessor<T>> m_processor;
	std::shared_ptr<MultiThreadFifo<T>> m_fifoIn;
};

template <typename T1, typename T2>
class ConsumerProducer : public Worker
{
public:
	ConsumerProducer() = delete;
	ConsumerProducer(const ConsumerProducer &other) = delete;
	ConsumerProducer(std::shared_ptr<ConsumerProducerProcessor<T1, T2>> processor, const std::vector<std::shared_ptr<MultiThreadFifo<T2>>> & fifosOut) :
		Worker(), m_processor(processor), m_fifosOut(fifosOut)
	{
		m_fifoIn = std::make_shared<MultiThreadFifo<T1>>();
	}
	virtual ~ConsumerProducer(){}

	virtual bool doWork()
	{
		if(!m_fifoIn->empty())
		{
			T1 dataIn = m_fifoIn->pop();
			m_processor->setInput(dataIn);
			m_processor->run();
			T2 dataOut = m_processor->getOutput();
			for (auto & f : m_fifosOut)
			{
				f->push(dataOut);
			}
			return false;
		}
		return true;
	}

	std::shared_ptr<MultiThreadFifo<T1>> getFifoIn() { return m_fifoIn; }

private:
	std::shared_ptr<ConsumerProducerProcessor<T1, T2>> m_processor;
	std::shared_ptr<MultiThreadFifo<T1>> m_fifoIn;
	std::vector<std::shared_ptr<MultiThreadFifo<T2>>> m_fifosOut;
};



