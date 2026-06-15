#pragma once

#include <stdexcept>
#include <random>
#include <string>

#define MIN_LEN 8
#define OFFEST_INDX 0
#define CIPHER_LEN_INDX 4

class OTPCryptoAlgorithm
{
private:
	std::string buildPacket(unsigned int offset, const std::string& cipherText) const;
	void writeUInt32(std::string& buffer, int index, unsigned int value) const;
	unsigned int readUInt32(const std::string& buffer, int index) const;

public:
	std::string encrypt(const std::string& message, const std::string& keyId) const;
	std::string decrypt(const std::string& packet, const std::string& keyId) const;

	std::string generatePad(unsigned int length) const;
};