#include "stdafx.h"

#include "VideoGrabber.h"

#include <thread>
#include <iostream>
#include <opencv2\imgproc\imgproc.hpp>

VideoGrabber::VideoGrabber(std::string fileName) : ProducerProcessor<cv::Mat>(), m_fileName(fileName)
{
	m_out = cv::Mat(0, 0, CV_8UC3);
	m_vc = cv::VideoCapture(m_fileName);
	m_numFrames = m_vc.get(CV_CAP_PROP_FRAME_COUNT);
}

void VideoGrabber::run()
{
	if (m_cnt >= m_numFrames)
	{
		m_cnt = 0;
		m_vc.set(CV_CAP_PROP_POS_FRAMES, m_cnt);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}
	cv::Mat tmpImg;
	m_vc >> tmpImg;
	m_out = tmpImg;
	++m_cnt;
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
}
