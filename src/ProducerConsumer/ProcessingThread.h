#pragma once

#include "Worker.h"
#include <thread>

class ProcessingThread final
{
public:
	ProcessingThread() = delete;
	ProcessingThread(const ProcessingThread &p) = delete;
	ProcessingThread(std::shared_ptr<Worker> worker) : m_worker(worker) {}
	~ProcessingThread() { stop(); }

	bool start()
	{
		if (m_bIsRunning)
			return false;

		m_bRun = true;
		std::thread t([this](){
			m_bIsRunning = true;
			try
			{
				while (m_bRun)
				{
					if (m_worker->doWork())
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(5));
					}
				}
				m_bIsRunning = false;
			}
			catch (...)
			{
				m_bIsRunning = false;
			}
		});
		t.detach();
		return true;
	}

	bool stop(unsigned long waitMS)
	{
		m_bRun = false;
		for (unsigned long cnt = 0; cnt < waitMS; ++cnt)
		{
			if (m_bIsRunning)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			else 
				return true;
		}
		return !m_bIsRunning;
	}

	bool stop()
	{
		m_bRun = false;
		while(m_bIsRunning)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		return !m_bIsRunning;
	}

	bool isRunning()
	{
		return m_bIsRunning;
	}


private:
	bool m_bRun = false;
	bool m_bIsRunning = false;
	
	std::shared_ptr<Worker> m_worker;
};

