#include "RoomAdminRequestHandler.h"


/*
	Constructor for RoomAdminRequestHandler.
	Input: loggedUser (const LoggedUser&), room (Room&)
	Output: none
*/

RoomAdminRequestHandler::RoomAdminRequestHandler(const LoggedUser& loggedUser, Room& room) : RoomRequestHandler(loggedUser, room)
{}

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

	if (requestInfo.id == CLOSE_ROOM_REQUEST || requestInfo.id == START_GAME_REQUEST || requestInfo.id == GET_ROOM_STATE_REQUEST || requestInfo.id == GET_PLAYERS_IN_ROOM_REQUEST || requestInfo.id == ADD_QUESTION_REQUEST)
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
			case GET_PLAYERS_IN_ROOM_REQUEST:
			{
				return getPlayersInRoom(requestInfo);
			}
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

	this->m_room.setStatus(Closed);

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

	Game& game = GameManager::getInstance().createGame(this->m_room, this->m_newQuestions);

	this->m_room.setStatus(InGame);
	response.status = Success;

	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = this->m_handlerFactory.createGameRequestHandler(game, this->m_user);

	return result;
}

/*
	Method to process the addQuestion request
	input: the request's information
	output: the request's result
*/

RequestResult RoomAdminRequestHandler::addQuestion(const RequestInfo& requestInfo)
{
	RequestResult result;
	AddQuestionResponse response;

	AddQuestionRequest request = JsonRequestPacketDeserializer::deserializeAddQuestionRequest(requestInfo.buffer);
	Question question(request.question, request.answers, request.correctAnswerId);
	this->m_newQuestions.push_back(question);

	response.status = Success;

	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user, this->m_room);

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
	result.newHandler = nullptr;

	return result;
}

