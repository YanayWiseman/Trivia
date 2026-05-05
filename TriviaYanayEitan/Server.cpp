#include "Server.h"


/*
	Function constructs server object.
	Input: None
	Output: None (constructor)
*/

Server::Server() : m_database(&SqliteDatabase::getInstance()), m_handlerFactory(RequestHandlerFactory::getInstance()), m_communicator(Communicator::getInstance()) {}


/*
	This Method is the main thread of the multi-threaded server
	input: no input values
	output: no return values
*/

void Server::run()
{
	// spawn the connector thread
	std::thread connector(&Communicator::startHandleRequests, &this->m_communicator);
	connector.detach();

	std::string str = "";

	// main thread loop
	while (true)
	{
		std::getline(std::cin, str);
		if (str == "EXIT")
		{
			std::cout << "EXIT was entered, exiting program." << std::endl;
			exit(0);
		}
	}
}


/*
	Function creates single instance of server.
	Input: None
	Output: New server instance (Server&)
*/

Server& Server::getInstance()
{
	static Server instance;
	return instance;
}
