#pragma once

#include "Processor.h"

#include <opencv2\core\core.hpp>

class Stopper :
	public ConsumerProcessor < cv::Mat >
{
public:
	Stopper() = delete;
	Stopper(bool &bStop) : m_bStop(bStop) {}
	virtual ~Stopper(){}

	virtual void run()
	{
		if (m_in.empty())
		{
			m_bStop = false;
		}
	}


private:
	bool &m_bStop;
};

