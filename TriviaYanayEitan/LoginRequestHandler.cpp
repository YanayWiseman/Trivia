#include "LoginRequestHandler.h"

/*
	Method to check if the request code is valid
	input: the request info
	output: true if valid and false otherwise
*/

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
	if (requestInfo.buffer[0] > 0 && requestInfo.buffer[0] < 6)  // TEMP
	{
		return true;
	}
	return false;
}

/*
	Method to handle the request (send it and return the result)
	input: the request info
	output: the request result
*/

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo) const
{
	// if request is relevant
	if (isRequestRelevant(requestInfo))
	{
		switch (requestInfo.buffer[0])
		{
			case LOGIN_REQUEST:
			{
				// return LoginRequestHandler::login();
			}
			case SIGNUP_REQUEST:
			{
				// return LoginRequestHandler::signup();
			}
		}
	}
	return RequestResult();
}