#include "RoomAdminRequestHandler.h"

/*
	Constructor for the RoomAdminRequestHandler object
	input: the logged user's name and the room's data
	output: no return values
*/

RoomAdminRequestHandler::RoomAdminRequestHandler(const LoggedUser& loggedUser, Room& room)
	: m_handlerFactory(RequestHandlerFactory::getInstance()), m_roomManager(RoomManager::getInstance()), m_user(loggedUser), m_room(room)
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
			case CLOSE_ROOM_REQUEST:
			{
				return closeRoom(requestInfo);
			}
			case START_GAME_REQUEST:
			{
				return startGame(requestInfo);
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
	Method to process the closeRoom request
	input: the request's information
	output: the request's result
*/

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& requestInfo) const
{
	RequestResult result;
	CloseRoomResponse response;

	int roomId = this->m_room.getData().id;
	this->m_roomManager.deleteRoom(roomId);
	response.status = Success;

	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);

	return result;
}

/*
	Method to process the startGame request
	input: the request's information
	output: the request's result
*/

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo) const
{
	RequestResult result;
	StartGameResponse response;

	this->m_room.setStatus(Open);
	response.status = Success;

	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<RoomAdminRequestHandler*>(this);

	return result;
}

/*
	Method to process the getRoomState request
	input: the request's information
	output: the request's result
*/

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& requestInfo) const
{
	RequestResult result;
	GetRoomStateResponse response;

	RoomData roomData = this->m_room.getData();

	if (this->m_roomManager.checkIfRoomExists(roomData.id))
	{
		response.answerTimeout = roomData.timePerQuestion;
		response.hasGameBegun = (roomData.status == Open);
		response.players = this->m_room.getAllUsers();
		response.questionsCount = roomData.numOfQuestionsInGame;
		response.status = 1;
	}
	else
	{
		response.status = 0;
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<RoomAdminRequestHandler*>(this);

	return result;
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
