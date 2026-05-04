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

	// we need to craft the room string
	std::string roomsStr = "";
	int i = 0;

	for (i = 0; i < getRoomsResponse.rooms.size(); i++)
	{
		roomsStr += getRoomsResponse.rooms[i].name;
		if (i != getRoomsResponse.rooms.size() - 1)
		{
			// the seperator will be &
			roomsStr += "&";
		}
	}

	response["Rooms"] = roomsStr;

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

	return JsonResponsePacketSerializer::serializeJson(response, CREATE_ROOM_RESPONSE);
}