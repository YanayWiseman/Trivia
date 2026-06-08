#pragma once

#include <thread>
#include <vector>

class ThreadPool
{
private:
	std::vector<std::thread> m_threads; 
};