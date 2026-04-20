#include "JsonRequestPacketDeserializer.h"

/*
	Method to deserialize a login request given the buffer
	input: the buffer
	output: the built login request struct
*/

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
	LoginRequest request;

	// message is built like this (according to the protocol)
	// code -> data len -> data

	if (JsonRequestPacketDeserializer::isBufferValid(buffer, 1))
	{
		// uint32_t guarantes 4 bytes only (data length)
		uint32_t dataLen =
			(buffer[1] << 24) |
			(buffer[2] << 16) |
			(buffer[3] << 8) |
			(buffer[4]);
		// now dataLen contains the 2-5 bytes of thhe buffer

		Buffer data(buffer.begin() + 1 + sizeof(uint32_t), buffer.begin() + 1 + sizeof(uint32_t) + dataLen);

		// create requestInfo
		RequestInfo requestInfo;
		requestInfo.id = buffer[0];
		requestInfo.buffer = data;

		time_t now = time(nullptr);
		requestInfo.receivalTime = now;

		try
		{
			json jsonData = json::parse(data);

			request.username = jsonData.at("username").get<std::string>();
			request.password = jsonData.at("password").get<std::string>();
		}
		catch (const std::exception& error)
		{
			throw std::runtime_error("JsonRequestPacketDeserializer::deserializeLoginRequest: error parsing json");
		}
	}
	else
	{
		throw std::exception("JsonRequestPacketDeserializer::deserializeLoginRequest: login code wasn't found");
	}

	return request;
}

/*
	Method to deserialize a signup request given the buffer
	input: the buffer
	output: the built login request struct
*/

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Buffer& buffer)
{
	SignupRequest request;

	if (JsonRequestPacketDeserializer::isBufferValid(buffer, 3))
	{
		uint32_t dataLen =
			(buffer[1] << 24) |
			(buffer[2] << 16) |
			(buffer[3] << 8) |
			(buffer[4]);
		// read the data
		Buffer data(buffer.begin() + 1 + sizeof(uint32_t), buffer.begin() + 1 + sizeof(uint32_t) + dataLen);

		// create requestInfo
		RequestInfo requestInfo;
		requestInfo.id = buffer[0];
		requestInfo.buffer = data;

		time_t now = time(nullptr);
		requestInfo.receivalTime = now;

		try
		{
			json jsonData = json::parse(data);

			request.username = jsonData.at("username").get<std::string>();
			request.password = jsonData.at("password").get<std::string>();
			request.email = jsonData.at("email").get<std::string>();
		}
		catch (const std::exception& error)
		{
			throw std::runtime_error("JsonRequestPacketDeserializer::deserializeSignupRequest: error parsing json");
		}
	}
	else
	{
		throw std::exception("JsonRequestPacketDeserializer::deserializeSignupRequest: signup code wasn't found");
	}

	return request;
}

/*
	Helper method to check if a buffer is valid by the protocol
	input: the buffer and the code it needs to have
	output: true if buffer is valid and false otherwise
*/

bool JsonRequestPacketDeserializer::isBufferValid(const Buffer& buffer, int code)
{
	// check if buffer is empty
	if (buffer.size() < sizeof(uint32_t) + 1)
	{
		return false;
	}

	// check if the buffer has its code
	if (buffer[0] != code)
	{
		return false;
	}

	uint32_t dataLen =
		(buffer[1] << 24) |
		(buffer[2] << 16) |
		(buffer[3] << 8) |
		(buffer[4]);

	// check the buffer's size match the data's actual len
	size_t offset = 1 + sizeof(uint32_t);
	if (offset + dataLen > buffer.size())
	{
		return false;
	}

	return true;
}