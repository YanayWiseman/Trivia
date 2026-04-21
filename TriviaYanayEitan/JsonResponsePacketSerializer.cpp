#include "JsonResponsePacketSerializer.h"


/*
	Function serializes Json login response into buffer.
	Input: loginResponse (const LoginResponse&)
	Output: Buffer containing login response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse& loginResponse)
{
	// Add status to JSON

	json response;
	response["status"] = loginResponse.status;

	std::string jsonStr = response.dump();

	// Serialize to buffer 

	Buffer buffer;
	buffer.push_back((Byte)LOGIN_RESPONSE);

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
	Function serializes Json signup response into buffer.
	Input: signupResponse (const SignupResponse&)
	Output: Buffer containing signup response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse& signupResponse)
{
	// Add status to JSON

	json response;
	response["status"] = signupResponse.status;

	std::string jsonStr = response.dump();

	// Serialize to buffer 

	Buffer buffer;
	buffer.push_back((Byte)SIGNUP_RESPONSE);

	// Convert size integer to 4 bytes of uchars

	int size = jsonStr.size();

	buffer.push_back((Byte)((size >> 24) & 0xFF));
	buffer.push_back((Byte)((size >> 16) & 0xFF));
	buffer.push_back((Byte)((size >> 8) & 0xFF));
	buffer.push_back((Byte)(size & 0xFF));

	// Convert json string to uchars

	int i = 0;

	for (i = 0; i < size; i++)
	{
		buffer.push_back((Byte)jsonStr[i]);
	}

	return buffer;
}


/*
	Function serializes Json error response into buffer.
	Input: errorResponse (const ErrorResponse&)
	Output: Buffer containing error response data according to the protocol.
*/

Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResponse)
{
	// Add status to JSON

	json response;
	response["message"] = errorResponse.error;

	std::string jsonStr = response.dump();

	// Serialize to buffer 

	Buffer buffer;
	buffer.push_back((Byte)ERROR_RESPONSE);

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
