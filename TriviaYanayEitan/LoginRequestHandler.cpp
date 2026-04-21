#include "LoginRequestHandler.h"

/*
	Method to check if the request code is valid
	input: the request info
	output: true if valid and false otherwise
*/

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
	if (requestInfo.buffer[0] == LOGIN_REQUEST || requestInfo.buffer[0] == SIGNUP_REQUEST)
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
				LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
				std::cout << "Calling Login..." << std::endl;
			}
			case SIGNUP_REQUEST:
			{
				SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
				std::cout << "Calling Signup..." << std::endl;
			}
		}
	}
	return RequestResult();
}