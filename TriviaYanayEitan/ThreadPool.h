#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

#define THREAD_POOL_SIZE 20

class ThreadPool
{
private:
	// fields
	std::vector<std::thread> m_threads;
	std::queue<std::function<void()>> m_tasks;
	std::mutex m_queueMutex;
	// used to put workers to sleep when there are no tasks, and wake them when one arrives
	// much cheaper than busy waiting (spinning in a loop checking the queue)
	std::condition_variable m_condition;
public:
	// c'tor
	ThreadPool(int numOfThreads);
};