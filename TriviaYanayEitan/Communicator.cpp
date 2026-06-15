#include "Communicator.h"
#include "RequestHandlerFactory.h"

/*
	Constructor method to construct the Communicator object by opening the socket
	input: no input values
	output: no return values
*/

Communicator::Communicator()
	: m_handlerFactory(RequestHandlerFactory::getInstance()), m_threadPool(new ThreadPool(THREAD_POOL_SIZE))
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

	delete this->m_threadPool;
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

		this->m_threadPool->enqueue([this, clientSocket]()
			{
				handleNewClient(clientSocket);
			});
	}
}


/*
	Helper to read exactly count bytes from a socket into dest
	input: the socket to read from, where to put the output, and the length
	output: false if the connection closed or an error occurred
*/

static bool recvExact(SOCKET sock, char* dest, int count)
{
	int total = 0;
	while (total < count)
	{
		int curr = recv(sock, dest + total, count - total, 0);
		if (curr <= 0)
		{
			return false;
		}
		total += curr;
	}

	return true;
}

/*
	Helper to send all of the given data into the socket
	input: the socket to send to and the data to insert into it
	output: false if the connection closed or an error occurred
*/

static bool sendAll(SOCKET sock, const std::string& data)
{
	int total = 0;
	int size = static_cast<int>(data.size());

	while (total < size)
	{
		int curr = send(sock, reinterpret_cast<const char*>(data.data()) + total, size - total, 0);
		if (curr == SOCKET_ERROR)
		{
			return false;
		}

		total += curr;
	}
	return true;
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
			char lenBuf[4];  // read 4 byte plaintext length prefix
			if (!recvExact(clientSocket, lenBuf, 4))
			{
				break;
			}

			uint32_t encLen =
				(static_cast<uint8_t>(lenBuf[0]) << 24) |
				(static_cast<uint8_t>(lenBuf[1]) << 16) |
				(static_cast<uint8_t>(lenBuf[2]) << 8) |
				(static_cast<uint8_t>(lenBuf[3]));

			if (encLen == 0 || encLen > 1024 * 1024)  // max 1 MB
			{
				break;
			}

			std::string encData(encLen, '\0');
			if (!recvExact(clientSocket, &encData[0], static_cast<int>(encLen)))
			{
				break;
			}

			// build Buffer
			Buffer buffer(encData.begin(), encData.end());

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

			std::string plainResponse(resultBuffer.response.begin(), resultBuffer.response.end());

			uint32_t outLen = static_cast<uint32_t>(plainResponse.size());
			char outLenBuffer[4];
			outLenBuffer[0] = static_cast<char>((outLen >> 24) & 0xFF);
			outLenBuffer[1] = static_cast<char>((outLen >> 16) & 0xFF);
			outLenBuffer[2] = static_cast<char>((outLen >> 8) & 0xFF);
			outLenBuffer[3] = static_cast<char>((outLen) & 0xFF);

			std::string outMsg(outLenBuffer, 4);
			outMsg += plainResponse;

			if (!sendAll(clientSocket, outMsg))
			{
				std::cerr << "send failed" << std::endl;
				break;
			}

			// Update handler if needed

			if (resultBuffer.newHandler != nullptr && resultBuffer.newHandler != handler)
			{
				std::lock_guard<std::mutex> lock(this->m_clientsMutex);
				delete this->m_clients[clientSocket];				if (send(clientSocket, reinterpret_cast<const char*>(resultBuffer.response.data()), (int)resultBuffer.response.size(), 0) == SOCKET_ERROR)
				{
					std::cerr << "send failed" << std::endl;
					break;
				}

				this->m_clients[clientSocket] = resultBuffer.newHandler;
				handler = resultBuffer.newHandler;
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
