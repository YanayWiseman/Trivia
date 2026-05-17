#include "RoomMemberRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"


/*
	Constructor for the RoomMemberRequestHandler object
	input: username (const std::string&), roomData (const RoomData&)
	output: no return values
*/

RoomMemberRequestHandler::RoomMemberRequestHandler(const LoggedUser& user, Room& room) : 
	m_handlerFactory(RequestHandlerFactory::getInstance()), m_user(user), 
	m_roomManager(RoomManager::getInstance()), m_room(room)
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

	if (requestInfo.id == LEAVE_ROOM_REQUEST || requestInfo.id == GET_ROOM_STATE_REQUEST)
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

	leaveRoomResponse.status;

	result.response = JsonResponsePacketSerializer::serializeResponse(leaveRoomResponse);

	if (leaveRoomResponse.status == Failiure)
	{
		result.newHandler = const_cast<RoomMemberRequestHandler*>(this);

	}
	else
	{
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(leaveRoomResponse.username);
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

