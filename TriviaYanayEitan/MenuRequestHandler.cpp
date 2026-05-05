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

	if (requestInfo.id >= LOGOUT_REQUEST && requestInfo.id <= CREATE_ROOM_REQUEST)
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
				return signout(requestInfo);

			case GET_ROOMS_REQUEST:
				return getRooms(requestInfo);

			case GET_PLAYERS_IN_ROOM_REQUEST:
				return getPlayersInRoom(requestInfo);

			case GET_PERSONAL_STATS_REQUEST:
				return getPersonalStats(requestInfo);

			case HIGH_SCORE_REQUEST:
				return getHighScore(requestInfo);

			case JOIN_ROOM_REQUEST:
				return joinRoom(requestInfo);

			case CREATE_ROOM_REQUEST:
				return createRoom(requestInfo);

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
	Method to process the logout request
	input: the request's information
	output: the request's result
*/

RequestResult MenuRequestHandler::signout(const RequestInfo& requestInfo) const
{
	RequestResult result;
	LogoutResponse response;

	std::string username = this->m_user.getUsername();

	this->m_handlerFactory.getLoginmanager().logout(username);
	response.status = Success;

	// build RequestResult struct
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<MenuRequestHandler*>(this);

	return result;
}

/*
	Method to process the getRooms request
	input: the request's information
	output: the request's result
*/

RequestResult MenuRequestHandler::getRooms(const RequestInfo& requestInfo) const
{
	RequestResult result;

	//TODO

	return result;
}

/*
	Method to process the getPlayersInRoom request
	input: the request's information
	output: the request's result
*/

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& requestInfo) const
{
	RequestResult result;
	GetPlayersInRoomResponse response;

	GetPlayersInRoomRequest request = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(requestInfo.buffer);
	int roomId = request.roomId;

	response.players = this->m_handlerFactory.getRoomManager().getRoom(roomId).getAllUsers();

	// build RequestResult struct
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<MenuRequestHandler*>(this);

	return result;
}

/*
	Method to process the getPersonalStats request
	input: the request's information
	output: the request's result
*/

RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& requestInfo) const
{
	RequestResult result;

	//TODO

	return result;
}

/*
	Method to process the getHighScore request
	input: the request's information
	output: the request's result
*/

RequestResult MenuRequestHandler::getHighScore(const RequestInfo& requestInfo) const
{
	RequestResult result;

	//TODO

	return result;
}

/*
	Method to process the joinRoom request
	input: the request's information
	output: the request's result
*/

RequestResult MenuRequestHandler::joinRoom(const RequestInfo& requestInfo) const
{
	RequestResult result;

	JoinRoomResponse response;

	JoinRoomRequest request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(requestInfo.buffer);
	int roomId = request.roomId;

	response.status = this->m_handlerFactory.getRoomManager().getRoom(roomId).addUser(this->m_user);

	// build RequestResult struct
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<MenuRequestHandler*>(this);

	return result;
}

/*
	Method to process the createRoom request
	input: the request's information
	output: the request's result
*/

RequestResult MenuRequestHandler::createRoom(const RequestInfo& requestInfo) const
{
	RequestResult result;

	CreateRoomResponse response;

	CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(requestInfo.buffer);
	
	// craft room data
	RoomData roomData;
	roomData.id = request.id;
	roomData.maxPlayers = request.maxUsers;
	roomData.name = request.roomName;
	roomData.numOfQuestionsInGame = request.questionsCount;
	roomData.timePerQuestion = request.answerTimeout;
	roomData.status = Closed;

	response.status = this->m_handlerFactory.getRoomManager().createRoom(this->m_user, roomData);

	// build RequestResult struct
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<MenuRequestHandler*>(this);

	return result;
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
