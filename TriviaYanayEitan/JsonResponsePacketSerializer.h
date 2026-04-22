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
	// methods
	static Buffer serializeJson(const json& json, const int code);
	static Buffer serializeResponse(const ErrorResponse& errorResponse);
	static Buffer serializeResponse(const LoginResponse& loginResponse);
	static Buffer serializeResponse(const SignupResponse& signupResponse);
};