#pragma once

#include "Processor.h"

#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

class VideoGrabber : public ProducerProcessor<cv::Mat>
{
public:
	VideoGrabber() = delete;
	VideoGrabber(std::string fileName);
	virtual ~VideoGrabber() {}
	virtual void run();

private:
	std::string m_fileName;
	double m_cnt = 0;
	double m_numFrames = 0;
	cv::VideoCapture m_vc;	
};

