#include "RoomRequestHandler.h"
#include "MenuRequestHandler.h"

/*
	Function constructs RoomRequestHandler object.
	Input: loggedUser (const LoggedUser&), room (Room&)
	Output: None (constructor)
*/

RoomRequestHandler::RoomRequestHandler(const LoggedUser& user, Room& room) :
	m_handlerFactory(RequestHandlerFactory::getInstance()), m_user(user),
	m_roomManager(RoomManager::getInstance()), m_room(room)
{
}


/*
	Function returns room state.
	Input: info (const RequestInfo& info)
	Output: Request result (RequestResult)
*/

RequestResult RoomRequestHandler::getRoomState(const RequestInfo& info) const
{
	RequestResult result;
	GetRoomStateResponse getRoomStateResponse;
	RoomData roomState;

	try
	{
		roomState = this->m_room.getData();

		getRoomStateResponse.answerTimeout = roomState.timePerQuestion;
		getRoomStateResponse.hasGameBegun = roomState.status;
		getRoomStateResponse.players = this->m_room.getAllUsers();
		getRoomStateResponse.questionsCount = roomState.numOfQuestionsInGame;
		getRoomStateResponse.status = Success;

		if (roomState.status == Closed)
		{
			result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
		}
		else
		{
			result.newHandler = const_cast<RoomRequestHandler*>(this);
		}
	}
	catch (const std::exception& error)
	{
		getRoomStateResponse.status = Failiure;
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(getRoomStateResponse);

	return result;
}


/*
	Method to process the getPlayersInRoom request
	input: the request's information
	output: the request's result
*/

RequestResult RoomRequestHandler::getPlayersInRoom(const RequestInfo& requestInfo) const
{
	RequestResult result;
	GetPlayersInRoomResponse response;

	try
	{
		GetPlayersInRoomRequest request = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(requestInfo.buffer);

		response.status = Success;
		response.players = this->m_handlerFactory.getRoomManager().getRoom(request.roomId).getAllUsers();
	}
	catch (...)
	{

		response.status = Failiure;
		response.players = std::vector<std::string>();
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<RoomRequestHandler*>(this);

	return result;
}
