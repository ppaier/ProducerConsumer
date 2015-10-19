#pragma once

#include "Processor.h"

#include <vector>
#include <opencv2\core\core.hpp>
#include <opencv2\features2d\features2d.hpp>

struct DetectorResult
{
	cv::Mat3b img;
	std::vector<cv::KeyPoint> keypoints;
};

class Detector :
	public ConsumerProducerProcessor<cv::Mat, DetectorResult>
{
public:
	Detector() = delete;
	Detector(cv::Ptr<cv::Feature2D>  fd);
	virtual ~Detector();

	virtual void run();

private:
	cv::Ptr<cv::Feature2D>  m_fd;

};

class KeypointDisplayer : public ConsumerProcessor <DetectorResult>
{
public:
	KeypointDisplayer() : ConsumerProcessor <DetectorResult>() {}
	virtual ~KeypointDisplayer(){}
	virtual void run();
};

