#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include <iostream>
#include <exception>

int main(void)
{
	try
	{
		WSAInitializer wsaInit;
		Server myServer;

		myServer.serve(9000);
	}
	catch (std::exception& error)
	{
		std::cout << "Error occured: " << error.what() << std::endl;
	}

	return 0;
}