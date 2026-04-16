#pragma once

#include <map>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

#include "Server.h"
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"

class Communicator
{
private:
	// fields
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	std::mutex m_clientsMutex;

	// methods
	void bindAndListen();
	void handleNewClient(SOCKET socket);

public:
	// methods
	Communicator();
	~Communicator();

	void startHandleRequests();
};