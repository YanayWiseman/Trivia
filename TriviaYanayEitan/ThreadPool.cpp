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
		// &ThreadPool::threadLoop is a pointer to the thread function
		// 'this' passes the current object as the implicit first argument
		this->m_threads.emplace_back(&ThreadPool::threadLoop, this);
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

/*
	This is the method that every thread runs forever, the thread is either sleeping(blocked by wait())
	or working (running one task and going back to sleep)
	input: No input values (except from the default this* pointer passed by the thread initialization in the c'tor)
	output: No return values
*/

void ThreadPool::threadLoop()
{
	while (true)
	{
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> locker(this->m_queueMutex);

			// sleep until a task in enqueued or the pool is stopping
			this->m_condition.wait(locker, [this]()
				{
					return this->m_stop || !this->m_tasks.empty();
				});

			// was woken up because of the d'tor
			if (this->m_stop && this->m_tasks.empty())
			{
				return;
			}

			task = std::move(this->m_tasks.front());
			this->m_tasks.pop();
		}

		// we run the task outside if the mutex's scope to allow other threads to work simultaniously
		task();
	}
}