#include "RoomAdminRequestHandler.h"

/*
	Constructor for the MenuRequestHandler object
	input: the handler factory and the logged user's name
	output: no return values
*/

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& handlerFactory, const std::string& username, const RoomData& roomData)
	: m_handlerFactory(handlerFactory), m_user(username), m_room(roomData)
{

}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{

}
RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo) const
{

}