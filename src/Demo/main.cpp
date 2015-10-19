// Tracking.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "VideoGrabber.h"
#include "LineFollowing.h"
#include "Detector.h"
#include "Stopper.h"
#include "ProcessingThread.h"

#include <memory>

#include <opencv2\xfeatures2d\nonfree.hpp>

void LineFollowingDemo();
void DetectorDemo();
void StopperDemo();

int _tmain(int argc, _TCHAR* argv[])
{
	DetectorDemo();
	return 0;
}

void LineFollowingDemo()
{

	std::shared_ptr<ConsumerProcessor<LineFollowingResult>> displayer = std::make_shared<LineDisplayer>();
	std::shared_ptr<Consumer<LineFollowingResult>> consumer = std::make_shared<Consumer<LineFollowingResult>>(displayer);

	std::vector<std::shared_ptr<MultiThreadFifo<LineFollowingResult>>> fifosConsumerProducer = { consumer->getFifoIn() };
	std::shared_ptr<ConsumerProducerProcessor<cv::Mat, LineFollowingResult>> lineFollowing = std::make_shared<LineFollowing>();
	std::shared_ptr<ConsumerProducer<cv::Mat, LineFollowingResult>> consumerProducer = std::make_shared<ConsumerProducer<cv::Mat, LineFollowingResult>>(lineFollowing, fifosConsumerProducer);

	std::vector<std::shared_ptr<MultiThreadFifo<cv::Mat>>> fifosProducer = { consumerProducer->getFifoIn() };
	std::shared_ptr<ProducerProcessor<cv::Mat>> vg = std::make_shared<VideoGrabber>("D:\\Eigene Dateien\\Eigene Projekte\\OpenCVSamples\\data\\grab.avi");
	std::shared_ptr<Worker> producer = std::make_shared<Producer<cv::Mat>>(vg, fifosProducer);

	ProcessingThread pt(producer);
	ProcessingThread cpt(consumerProducer);
	ProcessingThread ct(consumer);
	ct.start();
	cpt.start();
	pt.start();

	std::this_thread::sleep_for(std::chrono::milliseconds(10000));

}

void DetectorDemo()
{

	std::shared_ptr<ConsumerProcessor<DetectorResult>> displayer = std::make_shared<KeypointDisplayer>();
	std::shared_ptr<Consumer<DetectorResult>> consumer = std::make_shared<Consumer<DetectorResult>>(displayer);

	cv::Ptr<cv::Feature2D> sift = cv::xfeatures2d::SIFT::create();
	cv::Ptr<cv::Feature2D> surf = cv::xfeatures2d::SURF::create();

	std::vector<std::shared_ptr<MultiThreadFifo<DetectorResult>>> fifosConsumerProducer = { consumer->getFifoIn() };
	std::shared_ptr<ConsumerProducerProcessor<cv::Mat, DetectorResult>> detector1 = std::make_shared<Detector>(sift);
	std::shared_ptr<ConsumerProducerProcessor<cv::Mat, DetectorResult>> detector2 = std::make_shared<Detector>(surf);
	std::shared_ptr<ConsumerProducer<cv::Mat, DetectorResult>> consumerProducer1 = std::make_shared<ConsumerProducer<cv::Mat, DetectorResult>>(detector1, fifosConsumerProducer);
	std::shared_ptr<ConsumerProducer<cv::Mat, DetectorResult>> consumerProducer2 = std::make_shared<ConsumerProducer<cv::Mat, DetectorResult>>(detector2, fifosConsumerProducer);

	std::vector<std::shared_ptr<MultiThreadFifo<cv::Mat>>> fifosProducer = { consumerProducer1->getFifoIn(), consumerProducer2->getFifoIn() };
	std::shared_ptr<ProducerProcessor<cv::Mat>> vg = std::make_shared<VideoGrabber>("D:\\Eigene Dateien\\Eigene Projekte\\OpenCVSamples\\data\\grab.avi");
	std::shared_ptr<Worker> producer = std::make_shared<Producer<cv::Mat>>(vg, fifosProducer);

	ProcessingThread pt(producer);
	ProcessingThread cpt1(consumerProducer1);
	ProcessingThread cpt2(consumerProducer2);
	ProcessingThread ct(consumer);

	ct.start();
	cpt1.start();
	cpt2.start();
	pt.start();

	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}


void StopperDemo()
{
	bool bRun = true;

	std::shared_ptr<Stopper> stopper = std::make_shared<Stopper>(bRun);
	std::shared_ptr<Consumer<cv::Mat>> consumerStopper = std::make_shared<Consumer<cv::Mat>>(stopper);

	std::vector<std::shared_ptr<MultiThreadFifo<cv::Mat>>> fifosProducer = { consumerStopper->getFifoIn() };
	std::shared_ptr<ProducerProcessor<cv::Mat>> vg = std::make_shared<VideoGrabber>("D:\\Eigene Dateien\\Eigene Projekte\\OpenCVSamples\\data\\grab.avi");
	std::shared_ptr<Worker> producer = std::make_shared<Producer<cv::Mat>>(vg, fifosProducer);

	ProcessingThread pt(producer);
	ProcessingThread cst(consumerStopper);
	pt.start();

	while (bRun)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

}