#pragma once

#include "ResponseStructs.h"
#include "json.hpp"
#include "typedefs.h"

using json = nlohmann::json;

class JsonResponsePacketSerializer
{
public:
	// methods
	Buffer serializeResponse(ErrorResponse errorResponse);
	Buffer serializeResponse(LoginResponse loginResponse);
	Buffer serializeResponse(SignupResponse signupResponse);
};