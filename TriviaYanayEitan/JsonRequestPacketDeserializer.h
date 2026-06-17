#pragma once

#include <exception>
#include <cstring>
#include "HandlersStructs.h"
#include "RequestStructs.h"
#include "json.hpp"
#include "typedefs.h"
#include "Constants.h"

using json = nlohmann::json;

class JsonRequestPacketDeserializer
{
public:
	// c'tor
	JsonRequestPacketDeserializer() = default;

	// methods

	static int getBufferLength(const Buffer& buffer);

	//V1
	static LoginRequest deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);

	//V2
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(const Buffer& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const Buffer& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const Buffer& buffer);

	//V4
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(const Buffer& buffer);

	//V4.0.1
	static AddQuestionRequest deserializeAddQuestionRequest(const Buffer& buffer);
	static StartGameRequest deserializeStartGameRequest(const Buffer& buffer);

private:
	// helper methods
	static json extractJson(const Buffer& buffer, int expectedCode);
	static bool isBufferValid(const Buffer& buffer, int code);

	static JsonRequestPacketDeserializer& getInstance();

	JsonRequestPacketDeserializer(const JsonRequestPacketDeserializer& other) = delete;
	JsonRequestPacketDeserializer& operator=(const JsonRequestPacketDeserializer& other) = delete;
};