#pragma once

#include <vector>
#include "ResponseStructs.h"
#include "json.hpp"
#include "typedefs.h"

using json = nlohmann::json;

class JsonResponsePacketSerializer
{
public:
	// methods
	static Buffer serializeResponse(const ErrorResponse& errorResponse);
	static Buffer serializeResponse(const LoginResopnse& loginResponse);
	static Buffer serializeResponse(const SignupResopnse& signupResponse);
};