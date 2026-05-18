#include "RoomRequestHandler.h"


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
	}
	catch (const std::exception& error)
	{
		getRoomStateResponse.status = Failiure;
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(getRoomStateResponse);
	result.newHandler = const_cast<RoomRequestHandler*>(this);

	return result;
}


