#include "RoomMemberRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"



/*
	Constructor for RoomMemberRequestHandler.
	Input: loggedUser (const LoggedUser&), room (Room&)
	Output: none
*/

RoomMemberRequestHandler::RoomMemberRequestHandler(const LoggedUser& loggedUser, Room& room) : RoomRequestHandler(loggedUser, room)
{}

/*
	Method to check if the request code is valid
	input: the request info
	output: true if valid and false otherwise
*/

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
	if (requestInfo.buffer.size() == 0)
	{
		return false;
	}

	if (requestInfo.id == LEAVE_ROOM_REQUEST || requestInfo.id == GET_ROOM_STATE_REQUEST || requestInfo.id == GET_PLAYERS_IN_ROOM_REQUEST)
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

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo) const
{
	if (!isRequestRelevant(requestInfo))
	{
		return getErrorResponse("error: Invalid request!");
	}

	try
	{
		switch (requestInfo.id)
		{
		case GET_PLAYERS_IN_ROOM_REQUEST:
		{
			return getPlayersInRoom(requestInfo);
		}
		case LEAVE_ROOM_REQUEST:
		{
			return leaveRoom(requestInfo);
		}
		case GET_ROOM_STATE_REQUEST:
		{
			return getRoomState(requestInfo);
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
	Function removes user from the room.
	Input: info (const RequestInfo&)
	Output: Request result (RequestResult)
*/

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& info) const
{
	RequestResult result;
	LeaveRoomResponse leaveRoomResponse;

	try
	{
		this->m_room.removeUser(this->m_user);
		leaveRoomResponse.status = Success;
	}
	catch (const std::exception& error)
	{
		leaveRoomResponse.status = Failiure;
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(leaveRoomResponse);

	if (leaveRoomResponse.status == Failiure)
	{
		result.newHandler = const_cast<RoomMemberRequestHandler*>(this);
	}
	else
	{
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	}

	return result;
}


/*
	Function serializes and returns error response.
	Input: errorMsg (const std::string&)
	Output: Request result (RequestResult)
*/

RequestResult RoomMemberRequestHandler::getErrorResponse(const std::string& errorMsg) const
{
	RequestResult result;

	ErrorResponse errorResponse;
	errorResponse.error = errorMsg;

	result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	result.newHandler = nullptr;

	return result;
}

