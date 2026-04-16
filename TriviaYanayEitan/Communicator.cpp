#include "Communicator.h"


void Communicator::bindAndListen(int port)
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		throw std::exception(__FUNCTION__ " - bind");
	}

	// Start listening for incoming requests of clients
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw std::exception(__FUNCTION__ " - listen");
	}
	std::cout << "Listening on port " << port << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		handleNewClient();
	}
}

void Communicator::startHandleRequests()
{
	// this accepts the client and create a specific socket from server to this client
	// the process will not continue until a client connects to the server
	SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		throw std::exception(__FUNCTION__);
	}

	std::cout << "Client accepted. Server and client can speak" << std::endl;
	// the function that handle the conversation with the client
	handleNewClient(client_socket);
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	try
	{
		std::string s = "Welcome! What is your name (4 bytes)? ";
		send(clientSocket, s.c_str(), s.size(), 0);  // last parameter: flag. for us will be 0.

		char m[5];
		recv(clientSocket, m, 4, 0);
		m[4] = 0;
		std::cout << "Client name is: " << m << std::endl;

		s = "Bye";
		send(clientSocket, s.c_str(), s.size(), 0);

		// Closing the socket (in the level of the TCP protocol)
		closesocket(clientSocket);
	}
	catch (const std::exception& error)
	{
		closesocket(clientSocket);
	}
}