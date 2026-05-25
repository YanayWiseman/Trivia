#include "JsonRequestPacketDeserializer.h"



/*
	Function returns length of buffer
	Input: buffer (Buffer)
	Output: Length of buffer (int) 
*/

int JsonRequestPacketDeserializer::getBufferLength(const Buffer& buffer)
{
	return (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | (buffer[4]);
}



/*
	Method to deserialize a login request given the buffer
	input: the buffer
	output: the built login request struct
*/

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
	LoginRequest request;

	// message is built like this (according to the protocol)
	// code -> data len -> data

	json jsonData = JsonRequestPacketDeserializer::extractJson(buffer, LOGIN_REQUEST);
	try
	{
		request.username = jsonData.at("username").get<std::string>();
		request.password = jsonData.at("password").get<std::string>();
	}
	catch (const std::exception& error)
	{
		throw std::runtime_error("JsonRequestPacketDeserializer::deserializeLoginRequest: error parsing json");
	}

	return request;
}


/*
	Method to deserialize a signup request given the buffer
	input: the buffer
	output: the built login request struct
*/

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Buffer& buffer)
{
	SignupRequest request;

	json jsonData = JsonRequestPacketDeserializer::extractJson(buffer, SIGNUP_REQUEST);

	try
	{
		request.username = jsonData.at("username").get<std::string>();
		request.password = jsonData.at("password").get<std::string>();
		request.email = jsonData.at("email").get<std::string>();
		request.address = jsonData.at("address").get<std::string>();
		request.phone = jsonData.at("phone").get<std::string>();
		request.birthdate = jsonData.at("birthdate").get<std::string>();
	}
	catch (const std::exception& error)
	{
		throw std::runtime_error("JsonRequestPacketDeserializer::deserializeSignupRequest: error parsing json");
	}

	return request;
}


/*
	Helper method to check if a buffer is valid by the protocol
	input: the buffer and the code it needs to have
	output: true if buffer is valid and false otherwise
*/

bool JsonRequestPacketDeserializer::isBufferValid(const Buffer& buffer, int code)
{
	// check if buffer is empty
	if (buffer.size() < sizeof(uint32_t) + 1)
	{
		return false;
	}

	// check if the buffer has its code
	if (buffer[0] != code)
	{
		return false;
	}

	uint32_t dataLen = getBufferLength(buffer);

	// check the buffer's size match the data's actual len
	size_t offset = 1 + sizeof(uint32_t);
	if (offset + dataLen > buffer.size())
	{
		return false;
	}

	return true;
}

/*
	Method to extract the json code from the buffer
	input: the buffer and the expected code
	output: the extracted json
*/

json JsonRequestPacketDeserializer::extractJson(const Buffer& buffer, int expectedCode)
{
	if (!JsonRequestPacketDeserializer::isBufferValid(buffer, expectedCode))
	{
		throw std::runtime_error("invalid buffer or wrong request code");
	}

	uint32_t dataLen = getBufferLength(buffer);

	Buffer data(buffer.begin() + 1 + sizeof(uint32_t), buffer.begin() + 1 + sizeof(uint32_t) + dataLen);

	try
	{
		return json::parse(data);
	}
	catch (...)
	{
		throw std::runtime_error("Failed to parse JSON");
	}
}


/*
	Function creates single instance of JsonRequestPacketDeserializer.
	Input: None
	Output: New JsonRequestPacketDeserializer instance (JsonRequestPacketDeserializer&)
*/

JsonRequestPacketDeserializer& JsonRequestPacketDeserializer::getInstance()
{
	static JsonRequestPacketDeserializer instance;
	return instance;
}


/*
	Function returns deserialzed get players request from json buffer.
	Input: buffer (const Buffer&)
	Output: Deserialized get players request (GetPlayersInRoomRequest)
*/

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const Buffer& buffer)
{
	GetPlayersInRoomRequest request;

	json jsonData = JsonRequestPacketDeserializer::extractJson(buffer, GET_PLAYERS_IN_ROOM_REQUEST);

	try
	{
		request.roomId = jsonData.at("roomId").get<unsigned int>();
	}
	catch (const std::exception& error)
	{
		throw std::runtime_error("JsonRequestPacketDeserializer::deserializeGetPlayersRequest: error parsing json");
	}

	return request;
}


/*
	Function returns deserialzed join room request from json buffer.
	Input: buffer (const Buffer&)
	Output: Deserialized join room request (JoinRoomRequest)
*/

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Buffer& buffer)
{
	JoinRoomRequest request;

	json jsonData = JsonRequestPacketDeserializer::extractJson(buffer, JOIN_ROOM_REQUEST);

	try
	{
		request.roomId = jsonData.at("roomId").get<unsigned int>();
	}
	catch (const std::exception& error)
	{
		throw std::runtime_error("JsonRequestPacketDeserializer::deserializeJoinRoomRequest: error parsing json");
	}

	return request;
}


/*
	Function returns deserialized create room request from json buffer.
	Input: buffer (const Buffer&)
	Output: Deserialized create room request (CreateRoomRequest)
*/

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Buffer& buffer)
{
	CreateRoomRequest request;

	json jsonData = JsonRequestPacketDeserializer::extractJson(buffer, CREATE_ROOM_REQUEST);
	 
	try
	{
		request.roomName = jsonData.at("roomName").get<std::string>();
		request.maxUsers = jsonData.at("maxUsers").get<unsigned int>();
		request.questionsCount = jsonData.at("questionsCount").get<unsigned int>();
		request.answerTimeout = jsonData.at("answerTimeout").get<unsigned int>();
	}
	catch (const std::exception& error)
	{
		throw std::runtime_error("JsonRequestPacketDeserializer::deserializeCreateRoomRequest: error parsing json");
	}

	return request;
}

/*
	Method to deserialize a submit answer request given the buffer
	input: the buffer
	output: the built submit answer request struct
*/

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(const Buffer& buffer)
{
	SubmitAnswerRequest request;

	json jsonData = JsonRequestPacketDeserializer::extractJson(buffer, SUBMIT_ANSWER_REQUEST);

	try
	{
		request.answerId = jsonData.at("answerId").get<unsigned int>();
	}
	catch (const std::exception& error)
	{
		throw std::runtime_error("JsonRequestPacketDeserializer::deserializeCreateRoomRequest: error parsing json");
	}

	return request;
}