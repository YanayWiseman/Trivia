#pragma once

#include <WinSock2.h>
#include <Windows.h>

class Server
{
private:
	void acceptClient();
	void clientHandler(SOCKET clientSocket);

	SOCKET _serverSocket;

public:
	Server();
	~Server();
	void serve(int port);
};
