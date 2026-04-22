#include "JsonResponsePacketSerializer.h"


/*
	Function serializes json into Buffer.
	Input: json (const json&), code (const int)
	Output: Serialized json (Buffer)
*/

Buffer JsonResponsePacketSerializer::serializeJson(const json& json, const int code)
{
	std::string jsonStr = json.dump();

	// Serialize to buffer 

	Buffer buffer;
	buffer.push_back((Byte)code);

	// Convert size integer to 4 bytes of uchars

	int size = jsonStr.size();

	buffer.push_back((size >> 24) & 0xFF);
	buffer.push_back((size >> 16) & 0xFF);
	buffer.push_back((size >> 8) & 0xFF);
	buffer.push_back(size & 0xFF);

	// Convert json string to uchars

	int i = 0;

	for (i = 0; i < size; i++)
	{
		buffer.push_back((Byte)jsonStr[i]);
	}

	return buffer;
}


/*
	Function serializes Json login response into buffer.
	Input: loginResponse (const LoginResponse&)
	Output: Buffer containing login response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse& loginResponse)
{
	json response;
	response["status"] = loginResponse.status;

	return serializeJson(response, LOGIN_RESPONSE);
}


/*
	Function serializes Json signup response into buffer.
	Input: signupResponse (const SignupResponse&)
	Output: Buffer containing signup response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse& signupResponse)
{
	json response;
	response["status"] = signupResponse.status;

	return serializeJson(response, SIGNUP_RESPONSE);
}


/*
	Function serializes Json error response into buffer.
	Input: errorResponse (const ErrorResponse&)
	Output: Buffer containing error response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResponse)
{
	json response;
	response["message"] = errorResponse.error;

	return serializeJson(response, ERROR_RESPONSE);
}
