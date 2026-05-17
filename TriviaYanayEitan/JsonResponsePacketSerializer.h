#pragma once

#include <vector>
#include "ResponseStructs.h"
#include "json.hpp"
#include "typedefs.h"
#include "Constants.h"

using json = nlohmann::json;

class JsonResponsePacketSerializer
{
public:
	// c'tor
	JsonResponsePacketSerializer() = default;
	
	// methods

	//V1
	static Buffer serializeResponse(const ErrorResponse& errorResponse);
	static Buffer serializeResponse(const LoginResponse& loginResponse);
	static Buffer serializeResponse(const SignupResponse& signupResponse);

	//V2
	static Buffer serializeResponse(const LogoutResponse& loginResponse);
	static Buffer serializeResponse(const GetRoomsResponse& errorResponse);
	static Buffer serializeResponse(const GetPlayersInRoomResponse& signupResponse);
	static Buffer serializeResponse(const getHighScoreResponse& errorResponse);
	static Buffer serializeResponse(const getPersonalStatsResponse& loginResponse);
	static Buffer serializeResponse(const JoinRoomResponse& signupResponse);
	static Buffer serializeResponse(const CreateRoomResponse& signupResponse);

	//V3
	static Buffer serializeResponse(const CloseRoomResponse& closeRoomResponse);
	static Buffer serializeResponse(const StartGameResponse& startGameResponse);
	static Buffer serializeResponse(const GetRoomStateResponse& getRoomStateResponse);
	static Buffer serializeResponse(const LeaveRoomResponse& leaveRoomResponse);


	// singleton
	static JsonResponsePacketSerializer& getInstance();

	JsonResponsePacketSerializer(const JsonResponsePacketSerializer& other) = delete;
	JsonResponsePacketSerializer& operator=(const JsonResponsePacketSerializer& other) = delete;
private:
	// helper methods
	static Buffer serializeJson(const json& json, const int code);
	static std::string vectorToAmpersandString(const std::vector<std::string>& vec);
};