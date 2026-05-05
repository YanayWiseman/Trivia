#include "MenuRequestHandler.h"

/*
	Constructor for the MenuRequestHandler object
	input: the handler factory and the logged user's name
	output: no return values
*/

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handlerFactory, const std::string& username)
	: m_handlerFactory(handlerFactory), m_user(username)
{

}

/*
	Method to check if the request code is valid
	input: the request info
	output: true if valid and false otherwise
*/

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
	if (requestInfo.buffer.size() == 0)
	{
		return false;
	}

	if (//CHANGE THIS BASED ON THE MARCO RANGE YOU DECIDED IN DESIRIALIZATION)
		true)
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

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo) const
{
	RequestResult result;

	if (!isRequestRelevant(requestInfo))
	{
		return getErrorResponse("error: Invalid request!");
	}

	try
	{
		switch (requestInfo.id)
		{
		case LOGIN_REQUEST:
		{
			return login(requestInfo);
		}
		case SIGNUP_REQUEST:
		{
			return signup(requestInfo);
		}
		default:
		{
			return getErrorResponse("error: Wrong request format!");
		}
		}
	}
	catch (const std::exception& error)
	{
		return getErrorResponse(error.what());
	}
}

/*
	method serializes and returns error response.
	input: the errorMsg
	output: Request result
*/

RequestResult MenuRequestHandler::getErrorResponse(const std::string& errorMsg) const
{
	RequestResult result;

	ErrorResponse errorResponse;
	errorResponse.error = errorMsg;

	result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	result.newHandler = nullptr;

	return result;
}
