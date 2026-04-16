#include "Communicator.h"

/*
	Constructor method to construct the Communicator object by opening the socket
	input: no input values
	output: no return values
*/

Communicator::Communicator()
{
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->m_serverSocket == INVALID_SOCKET)
	{
		throw std::exception(__FUNCTION__ " - socket");
	}
}

/*
	Destructor method to destruct the Communicator object by closing the socket
	input: no input values
	output: no return values
*/

Communicator::~Communicator()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(this->m_serverSocket);
	}
	catch (...) {}
}

/*
	Method to bind and listen into the port we have decided (9000)
	input: no input values
	output: no return values
*/

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		throw std::exception(__FUNCTION__ " - bind");
	}

	// Start listening for incoming requests of clients
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw std::exception(__FUNCTION__ " - listen");
	}
	std::cout << "Listening on port " << PORT << std::endl;
}

/*
	This method the the thread method of the connector thread, accepting clients infinitly
	input: no input values
	output: no return values
*/

void Communicator::startHandleRequests()
{
	// first we set up the socket
	bindAndListen();

	while (true)
	{
		// this accepts the client and create a specific socket from server to this client
		// the process will not continue until a client connects to the server
		SOCKET clientSocket = accept(this->m_serverSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "Failed accepting client." << std::endl;
			continue;
		}

		// now we spawn the client thread function
		std::thread clientThread(&Communicator::handleNewClient, this, clientSocket);
		clientThread.detach();
	}
}

/*
	This method the the client thread's thread function, that operates the interaction between
	the server and the client
	input: the client's socket for communication
	output: no return values
*/

void Communicator::handleNewClient(SOCKET clientSocket)
{
	std::cout << "Client Connected!" << std::endl;
	try
	{
		std::string str = "Welcome! What is your name (4 bytes)? ";
		send(clientSocket, str.c_str(), str.size(), 0);  // last parameter: flag. for us will be 0.

		char m[5];
		recv(clientSocket, m, 4, 0);
		m[4] = 0;
		std::cout << "Client name is: " << m << std::endl;

		str = "Bye";
		send(clientSocket, str.c_str(), str.size(), 0);

		// Closing the socket (in the level of the TCP protocol)
		closesocket(clientSocket);
	}
	catch (const std::exception& error)
	{
		closesocket(clientSocket);
	}
}