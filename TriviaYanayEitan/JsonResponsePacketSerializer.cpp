#include "JsonResponsePacketSerializer.h"

/*
	Function serializes json into Buffer.
	Input: json (const json&), code (const int)
	Output: Serialized json (Buffer)
*/

Buffer JsonResponsePacketSerializer::serializeJson(const json& json, const int code)
{
	std::string jsonStr = json.dump();

	// Serialize to buffer 

	Buffer buffer;
	buffer.push_back((Byte)code);

	// Convert size integer to 4 bytes of uchars

	int size = jsonStr.size();

	buffer.push_back((size >> 24) & 0xFF);
	buffer.push_back((size >> 16) & 0xFF);
	buffer.push_back((size >> 8) & 0xFF);
	buffer.push_back(size & 0xFF);

	// Convert json string to uchars

	int i = 0;

	for (i = 0; i < size; i++)
	{
		buffer.push_back((Byte)jsonStr[i]);
	}

	return buffer;
}
 

/*
	Function serializes Json login response into buffer.
	Input: loginResponse (const LoginResponse&)
	Output: Buffer containing login response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse& loginResponse)
{
	json response;
	response["status"] = loginResponse.status;

	return serializeJson(response, LOGIN_RESPONSE);
}


/*
	Function serializes Json signup response into buffer.
	Input: signupResponse (const SignupResponse&)
	Output: Buffer containing signup response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse& signupResponse)
{
	json response;
	response["status"] = signupResponse.status;

	return serializeJson(response, SIGNUP_RESPONSE);
}


/*
	Function serializes Json error response into buffer.
	Input: errorResponse (const ErrorResponse&)
	Output: Buffer containing error response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResponse)
{
	json response;
	response["message"] = errorResponse.error;

	return serializeJson(response, ERROR_RESPONSE);
}


/*
	Function creates single instance of JsonResponsePacketSerializer.
	Input: None
	Output: New JsonResponsePacketSerializer instance (JsonResponsePacketSerializer&)
*/

JsonResponsePacketSerializer& JsonResponsePacketSerializer::getInstance()
{
	static JsonResponsePacketSerializer instance;
	return instance;
}

/*
	Method to serialize Json logout response into buffer.
	Input: LogoutResponse (const LogoutResponse&)
	Output: Buffer containing logout response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& logoutResponse)
{
	json response;
	response["status"] = logoutResponse.status;

	return JsonResponsePacketSerializer::serializeJson(response, LOGOUT_RESPONSE);
}

/*
	Method to serialize Json getRooms response into buffer.
	Input: GetRoomsResponse (const GetRoomsResponse&)
	Output: Buffer containing getRooms response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& getRoomsResponse)
{
	json response;

	json roomsArray = json::array();

	for (const auto& room : getRoomsResponse.rooms)
	{
		json roomJson;

		roomJson["id"] = room.id;
		roomJson["name"] = room.name;
		roomJson["maxPlayers"] = room.maxPlayers;
		roomJson["numOfQuestionsInGame"] = room.numOfQuestionsInGame;
		roomJson["timePerQuestion"] = room.timePerQuestion;
		roomJson["roomStatus"] = room.status;

		roomsArray.push_back(roomJson);
	}

	response["rooms"] = roomsArray;
	response["status"] = getRoomsResponse.status;

	return JsonResponsePacketSerializer::serializeJson(response, GET_ROOMS_RESPONSE);
}

/*
	Method to serialize Json GetPlayersInRoom response into buffer.
	Input: GetPlayersInRoomResponse (const GetPlayersInRoomResponse&)
	Output: Buffer containing GetPlayersInRoom response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResponse)
{
	json response;

	// we need to craft the players in room string
	response["PlayersInRoom"] = JsonResponsePacketSerializer::vectorToAmpersandString(getPlayersInRoomResponse.players);
	response["status"] = getPlayersInRoomResponse.status;

	return JsonResponsePacketSerializer::serializeJson(response, GET_PLAYERS_IN_ROOM_RESPONSE);
}

/*
	Method to serialize Json getHighScore response into buffer.
	Input: getHighScoreResponse (const getHighScoreResponse&)
	Output: Buffer containing getHighScore response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const getHighScoreResponse& getHighScoreResponse)
{
	json response;

	response["status"] = getHighScoreResponse.status;
	response["HighScore"] = JsonResponsePacketSerializer::vectorToAmpersandString(getHighScoreResponse.statistics);

	return JsonResponsePacketSerializer::serializeJson(response, HIGH_SCORE_RESPONSE);
}

/*
	Method to serialize Json getPersonalStats response into buffer.
	Input: getPersonalStatsResponse (const getPersonalStatsResponse&)
	Output: Buffer containing getPersonalStats response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const getPersonalStatsResponse& getPersonalStatsResponse)
{
	json response;

	response["status"] = getPersonalStatsResponse.status;
	response["UserStatistics"] = JsonResponsePacketSerializer::vectorToAmpersandString(getPersonalStatsResponse.statistics);

	return JsonResponsePacketSerializer::serializeJson(response, GET_PERSONAL_STATS_RESPONSE);
}

/*
	Method to serialize Json JoinRoom response into buffer.
	Input: JoinRoomResponse (const JoinRoomResponse&)
	Output: Buffer containing JoinRoom response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& joinRoomResponse)
{
	json response;
	response["status"] = joinRoomResponse.status;

	return JsonResponsePacketSerializer::serializeJson(response, JOIN_ROOM_RESPONSE);
}

/*
	Method to serialize Json CreateRoom response into buffer.
	Input: CreateRoomResponse (const CreateRoomResponse&)
	Output: Buffer containing CreateRoom response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& createRoomResponse)
{
	json response;
	response["status"] = createRoomResponse.status;
	response["roomId"] = createRoomResponse.roomId;

	return JsonResponsePacketSerializer::serializeJson(response, CREATE_ROOM_RESPONSE);
}

/*
	Helper method to convert a vector into a ampersand seperated string
	input: the vector
	output: the string
*/

std::string JsonResponsePacketSerializer::vectorToAmpersandString(const std::vector<std::string>& vector)
{
	size_t i = 0;

	std::string res = "";

	for (i = 0; i < vector.size(); i++)
	{
		res += vector[i];
		if (i != vector.size() - 1)
		{
			// the seperator will be &
			res += "&";
		}
	}

	return res;
}

/*
	Method to serialize Json CloseRoom response into buffer.
	Input: CloseRoomResponse (const CloseRoomResponse&)
	Output: Buffer containing CloseRoom response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& closeRoomResponse)
{
	json response;

	response["status"] = closeRoomResponse.status;

	return JsonResponsePacketSerializer::serializeJson(response, CLOSE_ROOM_RESPONSE);
}

/*
	Method to serialize Json StartGameResponse response into buffer.
	Input: StartGameResponse (const StartGameResponse&)
	Output: Buffer containing StartGame response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& startGameResponse)
{
	json response;

	response["status"] = startGameResponse.status;

	return JsonResponsePacketSerializer::serializeJson(response, START_GAME_RESPONSE);
}

/*
	Method to serialize Json GetRoomState response into buffer.
	Input: GetRoomStateResponse (const GetRoomStateResponse&)
	Output: Buffer containing GetRoomState response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& getRoomStateResponse)
{
	json response;

	response["status"] = getRoomStateResponse.status;
	response["hasGameBegun"] = getRoomStateResponse.hasGameBegun;
	response["timePerQuestion"] = getRoomStateResponse.answerTimeout;
	response["players"] = JsonResponsePacketSerializer::vectorToAmpersandString(getRoomStateResponse.players);
	response["questionCount"] = getRoomStateResponse.questionsCount;

	return JsonResponsePacketSerializer::serializeJson(response, GET_ROOM_STATE_RESPONSE);
}

/*
	Method to serialize Json LeaveRoom response into buffer.
	Input: LeaveRoomResponse (const LeaveRoomResponse&)
	Output: Buffer containing LeaveRoom response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& leaveRoomResponse)
{
	json response;

	response["status"] = leaveRoomResponse.status;

	return JsonResponsePacketSerializer::serializeJson(response, LEAVE_ROOM_RESPONSE);
}


/*
	Function inserts playerResults struct into json object.
	Input: j (json&), playerResults (const PlayerResults&)
	Output: None (void)
*/

void to_json(json& j, const PlayerResults& playerResults)
{
	j = json{
		{"username", playerResults.username},
		{"correctAnswerCount", playerResults.correctAnswerCount},
		{"wrongAnswerCount", playerResults.wrongAnswerCount},
		{"averageAnswerTime", playerResults.averageAnswerTime}
	};
}


/*
	Method to serialize Json GetGameResults response into buffer.
	Input: getGameResultsResponse (const GetGameResultsResponse&)
	Output: Buffer containing GetGameResults response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& getGameResultsResponse)
{
	json response;

	response["status"] = getGameResultsResponse.status;
	response["results"] = getGameResultsResponse.results;

	return JsonResponsePacketSerializer::serializeJson(response, GET_GAMRE_RESULTS_RESPONSE);
}


/*
	Method to serialize Json SubmitAnswer response into buffer.
	Input: submitAnswerResponse (const SubmitAnswerResponse&)
	Output: Buffer containing SubmitAnswer response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& submitAnswerResponse)
{
	json response;
	
	response["status"] = submitAnswerResponse.status;
	response["correctAnsId"] = submitAnswerResponse.correctAnswerId;

	return JsonResponsePacketSerializer::serializeJson(response, SUBMIT_ANSWER_RESPONSE);
}


/*
	Method to serialize Json GetQuestion response into buffer.
	Input: getQuestionResponse (const GetQuestionResponse&)
	Output: Buffer containing GetQuestion response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse& getQuestionResponse)
{
	json response;

	response["status"] = getQuestionResponse.status;
	response["question"] = getQuestionResponse.question;

	json answersJson = json::object();

	for (const auto& pair : getQuestionResponse.answers)
	{
		answersJson[std::to_string(pair.first)] = pair.second;
	}

	response["answers"] = answersJson;

	return JsonResponsePacketSerializer::serializeJson(response, GET_QUESTION_RESPONSE);
}

/*
	Method to serialize Json LeaveGame response into buffer.
	Input: leaveGameResponse (const LeaveGameResponse&)
	Output: Buffer containing LeaveGame response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& leaveGameResponse)
{
	json response;

	response["status"] = leaveGameResponse.status;

	return JsonResponsePacketSerializer::serializeJson(response, LEAVE_GAME_RESPONSE);
}

/*
	Method to serialize Json AddQuestionresponse into buffer.
	Input: addQuestionResponse (const AddQuestionResponse&)
	Output: Buffer containing AddQuestion response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const AddQuestionResponse& leaveGameResponse)
{
	json response;

	response["status"] = leaveGameResponse.status; 

	return JsonResponsePacketSerializer::serializeJson(response, ADD_QUESTION_RESPONSE);
}