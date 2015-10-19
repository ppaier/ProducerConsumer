#include "stdafx.h"

#include "Detector.h"

#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

Detector::Detector(cv::Ptr<cv::Feature2D> fd) : ConsumerProducerProcessor<cv::Mat, DetectorResult>(),
	m_fd(fd)
{

}

Detector::~Detector()
{
}

void Detector::run()
{
	cv::Mat1b img;
	cv::cvtColor(m_in, img, CV_BGR2GRAY);
	m_out.keypoints.clear();
	m_out.img = m_in;
	m_fd->detect(img, m_out.keypoints);
}

void KeypointDisplayer::run()
{
	cv::Mat3b imgColor;
	drawKeypoints(m_in.img, m_in.keypoints, imgColor);

	if (!m_in.img.empty())
	{
		cv::imshow("KeypointDetection", imgColor);
		cv::waitKey(1);
	}
}
