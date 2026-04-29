#pragma once

#include <WinSock2.h>
#include <map>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>

#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.h"

#define PORT 12334
#define BUFFER_SIZE 1024

class Communicator
{
private:
	// fields
	RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	std::mutex m_clientsMutex;

	// methods
	void bindAndListen();
	void handleNewClient(SOCKET socket);

public:
	// methods
	Communicator(RequestHandlerFactory& handlerFactory);
	~Communicator();

	void startHandleRequests();
};