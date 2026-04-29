#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <exception>
#include <iostream>
#include <string>
#include <thread>

#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"
#include "SqliteDatabase.h"


class Server
{
private:
	// fields
	Communicator& m_communicator;
	IDatabase* m_database;
	RequestHandlerFactory& m_handlerFactory;

	//methods
	Server();

public:
	// methods
	~Server() = default;

	void run();

	static Server& getInstance();

	Server(const Server& other) = delete;
	Server& operator=(const Server& other) = delete;
};
