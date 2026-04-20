#include "JsonResponsePacketSerializer.h"

Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResopnse& loginResponse)
{
	json response;

	response["status"] = loginResponse.status;

	std::vector<std::uint8_t> bResponse = json::to_bson(response);
	Buffer buffer = bResponse;

	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResopnse& signupResponse)
{
	json response;

	response["status"] = signupResponse.status;

	std::vector<std::uint8_t> bResponse = json::to_bson(response);
	Buffer buffer = bResponse;

	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResponse)
{
	json response;

	response["message"] = errorResponse.error;

	std::vector<std::uint8_t> bResponse = json::to_bson(response);
	Buffer buffer = bResponse;

	return buffer;
}
