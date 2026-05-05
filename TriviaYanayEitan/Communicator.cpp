#include "Communicator.h"
#include "RequestHandlerFactory.h"

/*
	Constructor method to construct the Communicator object by opening the socket
	input: no input values
	output: no return values
*/

Communicator::Communicator() : m_handlerFactory(RequestHandlerFactory::getInstance())
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
	{
		std::lock_guard<std::mutex> lock(this->m_printMutex);
		std::cout << "Client Connected!" << std::endl;
	}

	// add to the map
	IRequestHandler* handler = this->m_handlerFactory.createLoginRequestHandler();

	// now we use the comminicator's mutex and lock it in order to add it to the map
	{
		std::lock_guard<std::mutex> lock(this->m_clientsMutex);
		this->m_clients[clientSocket] = handler;
	}

	try
	{
		char cBuffer[BUFFER_SIZE];

		while (true)
		{
			int bytesRecieved = recv(clientSocket, cBuffer, sizeof(cBuffer) - 1, 0);

			if (bytesRecieved > 0)
			{
				// Create handler and turn buffer from chars to Bytes

				cBuffer[bytesRecieved] = '\0';

				Buffer buffer;
				int i = 0;

				for (i = 0; i < bytesRecieved; i++)
				{
					buffer.push_back((Byte)cBuffer[i]);
				}


				// Create requestInfo and handle request

				RequestInfo requestInfo;
				requestInfo.id = buffer[0];
				requestInfo.buffer = buffer;

				time_t now = time(nullptr);
				requestInfo.receivalTime = now;


				// Get current handler from map
				{
					std::lock_guard<std::mutex> lock(this->m_clientsMutex);
					handler = this->m_clients[clientSocket];
				}


				RequestResult resultBuffer = handler->handleRequest(requestInfo);

				if (resultBuffer.response.empty())
				{
					std::cerr << "Error: handler returned empty response" << std::endl;
					break;
				}

				if (send(clientSocket, reinterpret_cast<const char*>(resultBuffer.response.data()), (int)resultBuffer.response.size(), 0) == SOCKET_ERROR)
				{
					std::cerr << "send failed" << std::endl;
					break;
				}

				// Update handler if needed

				if (resultBuffer.newHandler != nullptr && resultBuffer.newHandler != handler)
				{
					std::lock_guard<std::mutex> lock(this->m_clientsMutex);
					delete this->m_clients[clientSocket];
					this->m_clients[clientSocket] = resultBuffer.newHandler;
					handler = resultBuffer.newHandler;
				}

			}
			else if (bytesRecieved == 0)
			{
				break;
			}
			else
			{
				std::cerr << "error: recv failed: " << WSAGetLastError() << std::endl;
				break;
			}
		}
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}

	closesocket(clientSocket);

	// remove from the map
	{
		std::lock_guard<std::mutex> lock(this->m_clientsMutex);
		delete this->m_clients[clientSocket];
		this->m_clients.erase(clientSocket);
	}

	{
		std::lock_guard<std::mutex> lock(this->m_printMutex);
		std::cout << "Client disconnected" << std::endl;
	}
}


/*
	Function creates single instance of Communicator.
	Input: None
	Output: New Communicator instance (Communicator&)
*/

Communicator& Communicator::getInstance()
{
	static Communicator instance;
	return instance;
}