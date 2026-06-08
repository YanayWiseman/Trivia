#include "ThreadPool.h"

/*
	Constructor for the ThreadPool object
	input: The pool's size (amount of threads it will contain and manage)
	output: no return values
*/

ThreadPool::ThreadPool(int numOfThreads)
{
	int i = 0;

	// fill up m_threads
	for (i = 0; i < numOfThreads; i++)
	{
		this->m_threads.emplace_back();  // TODO
	}
}

ThreadPool::~ThreadPool()
{

}