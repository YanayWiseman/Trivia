#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <exception>
#include <iostream>
#include <string>
#include <thread>

#include "Communicator.h"

#define PORT 9000

class Server
{
private:
	// fields
	Communicator m_communicator;

public:
	// methods
	Server() = default;
	~Server() = default;

	void run();
};
