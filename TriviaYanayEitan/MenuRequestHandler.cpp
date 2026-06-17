#include "MenuRequestHandler.h"

/*
	Constructor for the MenuRequestHandler object
	input: logged user's name
	output: no return values
*/

MenuRequestHandler::MenuRequestHandler(const LoggedUser& user)
	: m_handlerFactory(RequestHandlerFactory::getInstance()), m_user(user)
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

	if (requestInfo.id == JOIN_ONE_VS_ONE_QUEUE_REQUEST || requestInfo.id == GET_ONE_VS_ONE_STATUS_REQUEST || requestInfo.id == CANCEL_ONE_VS_ONE_QUEUE_REQUEST)
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

			case GET_PERSONAL_STATS_REQUEST:
				return getPersonalStats(requestInfo);

			case HIGH_SCORE_REQUEST:
				return getHighScore(requestInfo);

			case JOIN_ROOM_REQUEST:
				return joinRoom(requestInfo);

			case CREATE_ROOM_REQUEST:
				return createRoom(requestInfo);

			case JOIN_ONE_VS_ONE_QUEUE_REQUEST:
				return joinOneVsOneQueue(requestInfo);

			case GET_ONE_VS_ONE_STATUS_REQUEST:
				return getOneVsOneStatus(requestInfo);

			case CANCEL_ONE_VS_ONE_QUEUE_REQUEST:
				return cancelOneVsOneQueue(requestInfo);

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
	
	GetRoomsResponse response;

	response.rooms = this->m_handlerFactory.getRoomManager().getRooms();
	response.status = Success;

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

	getPersonalStatsResponse response;

	response.statistics = this->m_handlerFactory.getStatisticsManager().getUserStatistics(this->m_user.getUsername());
	response.status = Success;

	// build RequestResult struct
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<MenuRequestHandler*>(this);

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

	getHighScoreResponse response;

	response.statistics = this->m_handlerFactory.getStatisticsManager().getHighScores();
	response.status = Success;

	// build RequestResult struct
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<MenuRequestHandler*>(this);

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

	int roomId = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(requestInfo.buffer).roomId;
	Room& room = this->m_handlerFactory.getRoomManager().getRoom(roomId);

	if (room.getData().status == InGame || room.getData().status == Closed || room.isFull())
	{
		// game already started, cant join
		response.status = Failiure;
	}
	else
	{
		room.addUser(this->m_user);
		response.status = Success;
	}
	
	// build RequestResult struct
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	
	if (response.status == Success)
	{
		Room& room = this->m_handlerFactory.getRoomManager().getRoom(roomId);
		result.newHandler = this->m_handlerFactory.createRoomMemberRequestHandler(this->m_user, room);
	}
	else
	{
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	}

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

	RoomData roomData;

	roomData.maxPlayers = request.maxUsers;
	roomData.name = request.roomName;
	roomData.numOfQuestionsInGame = request.questionsCount;
	roomData.timePerQuestion = request.answerTimeout;
	roomData.status = Open;

	unsigned int roomId = this->m_handlerFactory.getRoomManager().createRoomWithRandomId(this->m_user, roomData);

	response.status = Success;
	response.roomId = roomId;

	result.response = JsonResponsePacketSerializer::serializeResponse(response);

	Room& room = this->m_handlerFactory.getRoomManager().getRoom(roomId);
	result.newHandler = this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user, room);

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
	result.newHandler = const_cast<MenuRequestHandler*>(this);

	return result;
}


/*
	Function calculates how many seconds are left until a 1v1 match starts.
	Input: matchData (const OneVsOneMatchData&)
	Output: Seconds until start (unsigned int)
*/

unsigned int MenuRequestHandler::getSecondsUntilStart(const OneVsOneMatchData& matchData) const
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	if (now >= matchData.startTime)
	{
		return 0;
	}

	return (unsigned int)std::chrono::duration_cast<std::chrono::seconds>(matchData.startTime - now).count();
}


/*
	Method to process the join 1v1 queue request.
	Input: the requests information
	Output: the requests result
*/

RequestResult MenuRequestHandler::joinOneVsOneQueue(const RequestInfo& requestInfo) const
{
	RequestResult result;
	OneVsOneStatusResponse response;
	OneVsOneMatchData matchData;

	OneVsOneQueueStatus queueStatus = OneVsOneManager::getInstance().joinQueue(this->m_user, matchData);

	response.status = Success;
	response.queueStatus = queueStatus;

	if (queueStatus == OneVsOneMatched)
	{
		response.roomId = matchData.roomId;
		response.gameId = matchData.gameId;
		response.questionsCount = ONE_VS_ONE_QUESTIONS_COUNT;
		response.answerTimeout = ONE_VS_ONE_TIME_PER_QUESTION;
		response.secondsUntilStart = this->getSecondsUntilStart(matchData);
		response.opponentUsername = matchData.opponentUsername;

		Game& game = GameManager::getInstance().getGame(matchData.gameId);
		result.newHandler = this->m_handlerFactory.createGameRequestHandler(game, this->m_user);
		OneVsOneManager::getInstance().removeMatch(this->m_user.getUsername());
	}
	else
	{
		response.roomId = 0;
		response.gameId = 0;
		response.questionsCount = 0;
		response.answerTimeout = 0;
		response.secondsUntilStart = 0;
		response.opponentUsername = "";

		result.newHandler = const_cast<MenuRequestHandler*>(this);
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(response);

	return result;
}


/*
	Method to process the get 1v1 queue status request.
	Input: the request's information
	Output: the request's result
*/

RequestResult MenuRequestHandler::getOneVsOneStatus(const RequestInfo& requestInfo) const
{
	RequestResult result;
	OneVsOneStatusResponse response;
	OneVsOneMatchData matchData;

	OneVsOneQueueStatus queueStatus = OneVsOneManager::getInstance().getStatus(this->m_user, matchData);

	response.status = Success;
	response.queueStatus = queueStatus;

	if (queueStatus == OneVsOneMatched)
	{
		response.roomId = matchData.roomId;
		response.gameId = matchData.gameId;
		response.questionsCount = ONE_VS_ONE_QUESTIONS_COUNT;
		response.answerTimeout = ONE_VS_ONE_TIME_PER_QUESTION;
		response.secondsUntilStart = this->getSecondsUntilStart(matchData);
		response.opponentUsername = matchData.opponentUsername;

		Game& game = GameManager::getInstance().getGame(matchData.gameId);
		result.newHandler = this->m_handlerFactory.createGameRequestHandler(game, this->m_user);
		OneVsOneManager::getInstance().removeMatch(this->m_user.getUsername());
	}
	else
	{
		response.roomId = 0;
		response.gameId = 0;
		response.questionsCount = 0;
		response.answerTimeout = 0;
		response.secondsUntilStart = 0;
		response.opponentUsername = "";

		result.newHandler = const_cast<MenuRequestHandler*>(this);
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(response);

	return result;
}


/*
	Method to process the cancel 1v1 queue request.
	Input: the request's information
	Output: the request's result
*/

RequestResult MenuRequestHandler::cancelOneVsOneQueue(const RequestInfo& requestInfo) const
{
	RequestResult result;
	OneVsOneStatusResponse response;

	OneVsOneManager::getInstance().cancelQueue(this->m_user);

	response.status = Success;
	response.queueStatus = OneVsOneNotQueued;
	response.roomId = 0;
	response.gameId = 0;
	response.questionsCount = 0;
	response.answerTimeout = 0;
	response.secondsUntilStart = 0;
	response.opponentUsername = "";

	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = const_cast<MenuRequestHandler*>(this);

	return result;
}