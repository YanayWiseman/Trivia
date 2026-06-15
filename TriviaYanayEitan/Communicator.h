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
#include "OTPCryptoAlgorithm.h"
#include "ThreadPool.h"

#define MAX_PACKET_SIZE (2 * 1024 * 1024)

#define PLAIN_PACKET 0
#define ENCRYPTED_PACKET 1

#define PORT 12334
#define BUFFER_SIZE 1024
#define THREAD_POOL_SIZE 20

class Communicator
{
private:
	// fields
	RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	std::mutex m_clientsMutex;
	std::mutex m_printMutex;
	ThreadPool* m_threadPool;


	// methods
	Communicator();
	void bindAndListen();
	void handleNewClient(SOCKET socket);
	static std::string buildPlainOuterPacket(const std::string& normalPacket);
	static std::string buildEncryptedOuterPacket(const std::string& encryptedPacket);
	static std::string getServerToClientKeyId(const std::string& clientToServerKeyId);
	static unsigned short readUInt16(const std::string& buffer, int index);

public:
	// methods
	~Communicator();

	static Communicator& getInstance();

	Communicator(const Communicator& other) = delete;
	Communicator& operator=(const Communicator& other) = delete;

	void startHandleRequests();
};