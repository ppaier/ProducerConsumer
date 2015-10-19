#pragma once

class Processor
{
public:
	Processor()	{}
	virtual ~Processor(){}

	virtual void run() = 0;

protected:
	unsigned long long m_times = 0;
};


template <typename T>
class ProducerProcessor : public Processor
{
public:
	ProducerProcessor() : Processor()	{}
	virtual ~ProducerProcessor(){}
	
	virtual T process()
	{
		run();
		return getOutput();
	}

	virtual T getOutput()
	{
		return m_out;
	}

protected:
	T m_out;
};


template <typename T>
class ConsumerProcessor : public Processor
{
public:
	ConsumerProcessor() : Processor() {}
	virtual ~ConsumerProcessor(){}
	
	virtual void process(const T &data)
	{
		setInput(data);
		run();
	}

	virtual void setInput(const T &data)
	{
		m_in = data;
	}
	
protected:
	T m_in;
};

template <typename T1, typename T2>
class ConsumerProducerProcessor : public Processor
{
public:
	ConsumerProducerProcessor() : Processor() {}
	virtual ~ConsumerProducerProcessor(){}

	virtual T2 process(const T1 &data)
	{
		setInput(data);
		run();
		return getOutput();
	}

	virtual void setInput(const T1 &data)
	{
		m_in = data;
	}

	virtual T2 getOutput()
	{
		return m_out;
	}

protected:
	T1 m_in;
	T2 m_out;
};