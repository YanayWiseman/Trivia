#pragma once

#include <exception>
#include <cstring>
#include "HandlersStructs.h"
#include "RequestStructs.h"
#include "json.hpp"
#include "typedefs.h"

using json = nlohmann::json;

class JsonRequestPacketDeserializer
{
public:
	// methods
	static LoginRequest deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);

private:
	// helper methods
	static bool isBufferValid(const Buffer& buffer, int code);
};