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
	// methods

	JsonRequestPacketDeserializer() = default;
	static LoginRequest deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);

private:
	// helper methods
	static json extractJson(const Buffer& buffer, int expectedCode);
	static bool isBufferValid(const Buffer& buffer, int code);

	static JsonRequestPacketDeserializer& getInstance();

	JsonRequestPacketDeserializer(const JsonRequestPacketDeserializer& other) = delete;
	JsonRequestPacketDeserializer& operator=(const JsonRequestPacketDeserializer& other) = delete;
};