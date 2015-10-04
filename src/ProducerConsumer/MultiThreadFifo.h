#pragma once
#include <queue>
#include <mutex>
#include <memory>
#include <exception>

template <typename T>
class MultiThreadFifo final
{

public:
	MultiThreadFifo() {}
	MultiThreadFifo(std::queue<T> fifo) : m_fifo(fifo) {}
	MultiThreadFifo(const MultiThreadFifo& other)
	{
		std::lock_guard<std::mutex> lg(other.m_mutex);
		m_fifo = other.m_fifo;
	}
	
	T pop()
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		if (!m_fifo.empty())
		{
			auto elem = m_fifo.front();
			m_fifo.pop();
			return elem;
		}
		else
		{
			throw std::exception("FiFo is empty!");
		}
	}

	void push(T elem)
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		m_fifo.push(elem);
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		return m_fifo.empty();
	}

private:
	std::mutex m_mutex;
	std::queue<T> m_fifo;

};

