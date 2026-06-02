#pragma once

#include <string>

class CryptoAlgorithm
{
public:
	// Methods
	virtual std::string encrypt(const std::string& plainText) const = 0;
	virtual std::string decrypt(const std::string& cipherText) const = 0;
};