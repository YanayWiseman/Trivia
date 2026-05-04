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

	if (requestInfo.id >= LOGOUT_RESPONSE && requestInfo.id <= CREATE_ROOM_RESPONSE)
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

}
