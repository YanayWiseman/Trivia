#include "JsonRequestPacketDeserializer.h"

/*
	Method to deserialize a login request given the buffer
	input: the buffer
	output: the built login request struct
*/

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
	// check if buffer is empty
	if (buffer.size() < sizeof(int))
	{
		throw std::exception("JsonRequestPacketDeserializer::deserializeLoginRequest: buffer empty");
	}

	// message is built like this (according to the protocol)
	// code -> data len -> data
	unsigned char code = buffer[0];  // first byte

	if (code == 1)
	{
		// uint32_t guarantes 4 bytes only (data length)
		uint32_t dataLen = 0;
		std::memcpy(&dataLen, &buffer[1], sizeof(uint32_t));
		// now dataLen contains the 2-5 bytes of thhe buffer
		
		// now we need to check that the dataLen isn't lying
		// by checking the buffer's size
		size_t offset = 1 + sizeof(uint32_t);
		if (offset + dataLen > buffer.size())
		{
			throw std::exception("JsonRequestPacketDeserializer::deserializeLoginRequest: data len was reported incorrect");
		}

		// read the data
		Buffer data(buffer.begin() + offset, buffer.begin() + offset + dataLen);
	}
	else
	{
		throw std::exception("JsonRequestPacketDeserializer::deserializeLoginRequest: login code wasn't found");
	}
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Buffer& buffer)
{
	return SignupRequest(); // TEMP
}