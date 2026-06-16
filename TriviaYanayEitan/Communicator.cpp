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
		while (true)
		{
			char lenBuffer[4];

			if (!recvExact(clientSocket, lenBuffer, 4))
			{
				break;
			}

			uint32_t outerPacketLength = (static_cast<uint8_t>(lenBuffer[0]) << 24) | (static_cast<uint8_t>(lenBuffer[1]) << 16) | (static_cast<uint8_t>(lenBuffer[2]) << 8) | static_cast<uint8_t>(lenBuffer[3]);

			if (outerPacketLength == 0 || outerPacketLength > MAX_PACKET_SIZE)
			{
				throw std::runtime_error("Invalid outer packet length");
			}

			std::string outerPacket(outerPacketLength, '\0');

			if (!recvExact(clientSocket, &outerPacket[0], static_cast<int>(outerPacketLength)))
			{
				break;
			}

			if (outerPacket.empty())
			{
				throw std::runtime_error("Received empty outer packet");
			}

			unsigned char packetMode = static_cast<unsigned char>(outerPacket[0]);

			bool encryptedRequest = false;
			std::string clientToServerKeyId;
			std::string normalRequest;

			if (packetMode == PLAIN_PACKET)
			{
				if (outerPacket.length() < 2)
				{
					throw std::runtime_error("Plain packet is too short");
				}

				normalRequest = outerPacket.substr(1);
			}
			else if (packetMode == ENCRYPTED_PACKET)
			{
				encryptedRequest = true;

				if (outerPacket.length() < 4)
				{
					throw std::runtime_error("Encrypted packet is too short");
				}

				unsigned short keyIdLength = readUInt16(outerPacket, 1);
				unsigned int encryptedPacketIndex = 3 + keyIdLength;

				if (keyIdLength == 0 || outerPacket.length() <= encryptedPacketIndex)
				{
					throw std::runtime_error("Encrypted packet contains an invalid key ID");
				}

				clientToServerKeyId = outerPacket.substr(3, keyIdLength);

				std::string encryptedRequestPacket = outerPacket.substr(encryptedPacketIndex);

				OTPCryptoAlgorithm otp;
				normalRequest = otp.decrypt(encryptedRequestPacket, clientToServerKeyId);
			}
			else
			{
				throw std::runtime_error("Unknown outer packet mode");
			}

			if (normalRequest.empty())
			{
				throw std::runtime_error("Normal request packet is empty");
			}

			Buffer buffer(normalRequest.begin(), normalRequest.end());

			// Create requestInfo and handle request
			RequestInfo requestInfo;
			requestInfo.id = buffer[0];
			requestInfo.buffer = buffer;
			requestInfo.receivalTime = time(nullptr);

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

			std::string normalResponse(resultBuffer.response.begin(), resultBuffer.response.end());
			std::string outerResponse;

			if (encryptedRequest)
			{
				std::string serverToClientKeyId = getServerToClientKeyId(clientToServerKeyId);

				OTPCryptoAlgorithm otp;
				std::string encryptedResponse = otp.encrypt(normalResponse, serverToClientKeyId);
				 
				outerResponse = buildEncryptedOuterPacket(encryptedResponse);
			}
			else
			{
				outerResponse = buildPlainOuterPacket(normalResponse);
			}

			uint32_t responseLength = static_cast<uint32_t>(outerResponse.size());

			char responseLengthBuffer[4];
			responseLengthBuffer[0] = static_cast<char>((responseLength >> 24) & 0xFF);
			responseLengthBuffer[1] = static_cast<char>((responseLength >> 16) & 0xFF);
			responseLengthBuffer[2] = static_cast<char>((responseLength >> 8) & 0xFF);
			responseLengthBuffer[3] = static_cast<char>(responseLength & 0xFF);

			std::string message(responseLengthBuffer, 4);
			message += outerResponse;

			if (!sendAll(clientSocket, message))
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
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}

	closesocket(clientSocket);

	// remove from the map
	{
		std::lock_guard<std::mutex> lock(this->m_clientsMutex);

		auto client = this->m_clients.find(clientSocket);

		if (client != this->m_clients.end())
		{
			delete client->second;
			this->m_clients.erase(client);
		}
	}

	{
		std::lock_guard<std::mutex> lock(this->m_printMutex);
		std::cout << "Client disconnected" << std::endl;
	}
}


/*
	Helper method wraps a normal response in a plaintext outer packet.
	input: the normal response packet
	output: the outer plaintext packet
*/

std::string Communicator::buildPlainOuterPacket(const std::string& normalPacket)
{
	std::string packet(1, static_cast<char>(PLAIN_PACKET));
	packet += normalPacket;

	return packet;
}


/*
	Helper method wraps an OTP packet in an encrypted outer packet.
	input: the encrypted OTP packet
	output: the outer encrypted packet
*/

std::string Communicator::buildEncryptedOuterPacket(const std::string& encryptedPacket)
{
	std::string packet(1, static_cast<char>(ENCRYPTED_PACKET));
	packet += encryptedPacket;

	return packet;
}


/*
	Helper method converts a client-to-server key ID into its matching server-to-client key ID.
	input: the client-to-server key ID
	output: the matching server-to-client key ID
*/

std::string Communicator::getServerToClientKeyId(const std::string& clientToServerKeyId)
{
	const std::string clientSuffix = "_c2s";
	const std::string serverSuffix = "_s2c";

	if (clientToServerKeyId.length() < clientSuffix.length() || clientToServerKeyId.substr(clientToServerKeyId.length() - clientSuffix.length()) != clientSuffix)
	{
		throw std::runtime_error("Invalid client-to-server key ID");
	}

	return clientToServerKeyId.substr(0, clientToServerKeyId.length() - clientSuffix.length()) + serverSuffix;
}


/*
	Helper method reads an unsigned short from a string in big-endian format.
	input: the string buffer and the index
	output: the unsigned short value
*/

unsigned short Communicator::readUInt16(const std::string& buffer, int index)
{
	return static_cast<unsigned short>((static_cast<unsigned char>(buffer[index]) << 8) | static_cast<unsigned char>(buffer[index + 1]));
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
