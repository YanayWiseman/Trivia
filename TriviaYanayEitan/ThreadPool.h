#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
private:
	// fields
	std::vector<std::thread> m_threads;
	std::queue<std::function<void()>> m_tasks;
	std::mutex m_queueMutex;
	bool m_stop;
	// used to put workers to sleep when there are no tasks, and wake them when one arrives
	// much cheaper than busy waiting (spinning in a loop checking the queue)
	std::condition_variable m_condition;
public:
	// c'tor & d'tor
	ThreadPool(int numOfThreads);
	~ThreadPool();
};