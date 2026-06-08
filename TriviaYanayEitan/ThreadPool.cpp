#include "ThreadPool.h"

/*
	Constructor method for the ThreadPool object
	input: The pool's size (amount of threads it will contain and manage)
	output: no return values
*/

ThreadPool::ThreadPool(int numOfThreads) : m_stop(false)
{
	int i = 0;

	// fill up m_threads
	for (i = 0; i < numOfThreads; i++)
	{
		this->m_threads.emplace_back();  // TODO
	}
}

/*
	Destructor method for the ThreadPool object
	input: No input values
	output: No return values
*/

ThreadPool::~ThreadPool()
{
	{
		// lock the queue mutex
		std::lock_guard<std::mutex> locker(this->m_queueMutex);
		this->m_stop = true;
	}

	// wake up all of the threads so they can see m_stop and stop
	this->m_condition.notify_all();

	for (std::thread& thread : this->m_threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}

/*
	Method adds a task to the task queue and notifies one thread
	input: The task to add
	output: no return values
*/

void ThreadPool::enqueue(std::function<void()> task)
{
	{
		std::lock_guard<std::mutex> locker(this->m_queueMutex);
		this->m_tasks.push(std::move(task));
	}
	
	this->m_condition.notify_one();
}

void ThreadPool::threadLoop()
{

}