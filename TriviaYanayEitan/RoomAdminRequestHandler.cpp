#include "RoomAdminRequestHandler.h"

/*
	Constructor for the MenuRequestHandler object
	input: the handler factory and the logged user's name
	output: no return values
*/

RoomAdminRequestHandler::RoomAdminRequestHandler(const std::string& username, const RoomData& roomData)
	: m_handlerFactory(RequestHandlerFactory::getInstance()), m_roomManager(RoomManager::getInstance()), m_user(username), m_room(roomData)
{

}

/*
	Method to check if the request code is valid
	input: the request info
	output: true if valid and false otherwise
*/

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
	if (requestInfo.buffer.size() == 0)
	{
		return false;
	}

	if (requestInfo.id >= CLOSE_ROOM_REQUEST && requestInfo.id <= GET_ROOM_STATE_REQUEST)
	{
		return true;
	}

	return false;
}


RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo) const
{

}