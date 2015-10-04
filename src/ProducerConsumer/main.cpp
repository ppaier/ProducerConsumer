// MultiThreadFifo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ProcessingThread.h"

#include <iostream>
#include <vector>

class Creator : public ProducerProcessor<double>
{
public: 
	Creator() {}
	virtual void run()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		m_out = cnt++;
	}
private:
	int cnt = 0;
};

class Doubler : public ConsumerProducerProcessor<double,double>
{

public:
	Doubler()  {}
	virtual void run()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
		m_out = m_in * 2;
	}
};

class Displayer : public ConsumerProcessor<double>
{

public:
	Displayer()  {}
	virtual void run()
	{
		std::cout << m_in << std::endl;
	}
};


int _tmain(int argc, _TCHAR* argv[])
{	
	std::shared_ptr<ConsumerProcessor<double>> displayer1 = std::make_shared<Displayer>();
	std::shared_ptr<ConsumerProcessor<double>> displayer2 = std::make_shared<Displayer>();
	std::shared_ptr<Consumer<double>> consumer1 = std::make_shared<Consumer<double>>(displayer1);
	std::shared_ptr<Consumer<double>> consumer2 = std::make_shared<Consumer<double>>(displayer2);
	
	std::vector<std::shared_ptr<MultiThreadFifo<double>>> fifosConsumerProducer;
	fifosConsumerProducer.push_back(consumer2->getFifoIn());

	std::shared_ptr<ConsumerProducerProcessor<double,double>> doubler = std::make_shared<Doubler>();
	std::shared_ptr<ConsumerProducer<double, double>> consumerProducer = std::make_shared<ConsumerProducer<double, double>>(doubler, fifosConsumerProducer);

	std::vector<std::shared_ptr<MultiThreadFifo<double>>> fifosProducer;
	fifosProducer.push_back(consumer1->getFifoIn());
	fifosProducer.push_back(consumerProducer->getFifoIn());

	std::shared_ptr<ProducerProcessor<double>> creator = std::make_shared<Creator>();	
	std::shared_ptr<Worker> producer = std::make_shared<Producer<double>>(creator, fifosProducer);

	ProcessingThread ptC1(consumer1);
	ProcessingThread ptC2(consumer2);
	ProcessingThread ptCP(consumerProducer);
	ProcessingThread ptP(producer);

	ptC1.start();
	ptC2.start();
	ptCP.start();
	ptP.start();

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	return 0;
}
