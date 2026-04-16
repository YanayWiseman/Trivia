#pragma once

#include <map>
#include <string>
#include <iostream>
#include "Server.h"
#include "IRequestHandler.h"

class Communicator
{
private:
	// fields
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;

	// methods
	void bindAndListen();
	void handleNewClient(SOCKET socket);

public:
	// methods
	Communicator();
	~Communicator();

	void startHandleRequests();
};