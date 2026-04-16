#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "Communicator.h"

class Server
{
private:
	// fields
	Communicator m_communicator;

public:
	// methods
	Server();
	~Server();

	void run();
};
