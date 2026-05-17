#include "RoomAdminRequestHandler.h"

/*
	Constructor for the RoomAdminRequestHandler object
	input: the logged user's name and the room's data
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

/*
	Method to handle the request (send it and return the result)
	input: the request info
	output: the request result
*/

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo) const
{
	// first check if the request is valid
	if (!isRequestRelevant(requestInfo))
	{
		return getErrorResponse("error: Invalid request!");
	}


	// then sort by request id to handle each request
	try
	{
		switch (requestInfo.id)
		{
		case LOGOUT_REQUEST:
		{
			return signout(requestInfo);
		}

		case GET_ROOMS_REQUEST:
		{
			return getRooms(requestInfo);
		}

		case GET_PLAYERS_IN_ROOM_REQUEST:
		{
			return getPlayersInRoom(requestInfo);
		}

		default:
			return getErrorResponse("error: Wrong request format!");
		}
	}
	catch (const std::exception& error)
	{
		return getErrorResponse(error.what());
	}
}

/*
	Method serializes and returns error response.
	input: the errorMsg
	output: Request result
*/

RequestResult RoomAdminRequestHandler::getErrorResponse(const std::string& errorMsg) const
{
	RequestResult result;

	ErrorResponse errorResponse;
	errorResponse.error = errorMsg;

	result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	result.newHandler = const_cast<RoomAdminRequestHandler*>(this);

	return result;
}
