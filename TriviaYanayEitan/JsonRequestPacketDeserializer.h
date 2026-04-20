#pragma once

#include "RequestStructs.h"
#include "json.hpp"
#include "typedefs.h"

using json = nlohmann::json;

class JsonRequestPacketDeserializer
{
public:
	// methods
	LoginRequest deserializeLoginRequest(Buffer buffer);
	SignupRequest deserializeSignupRequest(Buffer buffer);
};