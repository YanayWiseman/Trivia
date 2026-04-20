#include "JsonResponsePacketSerializer.h"

Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResopnse& loginResponse)
{
	json response;

	response["status"] = loginResponse.status;

	std::string jsonStr = response.dump();

	Buffer buffer;

	buffer.push_back((Byte)LOGIN_RESPONSE);

	int size = jsonStr.size();

	buffer.push_back((size >> 24) & 0xFF);
	buffer.push_back((size >> 16) & 0xFF);
	buffer.push_back((size >> 8) & 0xFF);
	buffer.push_back(size & 0xFF);

	int i = 0;

	for (i = 0; i < size; i++)
	{
		buffer.push_back((unsigned char)jsonStr[i]);
	}

	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResopnse& signupResponse)
{
	json response;

	response["status"] = signupResponse.status;

	std::string jsonStr = response.dump();

	Buffer buffer;

	buffer.push_back((Byte)LOGIN_RESPONSE);

	int size = jsonStr.size();

	buffer.push_back((size >> 24) & 0xFF);
	buffer.push_back((size >> 16) & 0xFF);
	buffer.push_back((size >> 8) & 0xFF);
	buffer.push_back(size & 0xFF);

	int i = 0;

	for (i = 0; i < size; i++)
	{
		buffer.push_back((unsigned char)jsonStr[i]);
	}

	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResponse)
{
	json response;

	response["message"] = errorResponse.error;

	std::string jsonStr = response.dump();

	Buffer buffer;

	buffer.push_back((Byte)LOGIN_RESPONSE);

	int size = jsonStr.size();

	buffer.push_back((size >> 24) & 0xFF);
	buffer.push_back((size >> 16) & 0xFF);
	buffer.push_back((size >> 8) & 0xFF);
	buffer.push_back(size & 0xFF);

	int i = 0;

	for (i = 0; i < size; i++)
	{
		buffer.push_back((unsigned char)jsonStr[i]);
	}

	return buffer;
}
