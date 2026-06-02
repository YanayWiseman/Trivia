#pragma once

#include <string>

// All of the inheriting classes might be differently implemented
// But all of them support this:
// Given a message m: decrypt(encrypt(m)) = m

class CryptoAlgorithm
{
public:
	// Methods
	virtual std::string encrypt(const std::string& message) const = 0;
	virtual std::string decrypt(const std::string& cipher) const = 0;
};