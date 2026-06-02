#pragma once

#include <random>
#include "CryptoAlgorithm.h"
#include "SqliteDatabase.h"

#define MIN_PRINTABLE_ASCII 32
#define MAX_PRINTABLE_ASCII 126

class OTPCryptoAlgorithm : public CryptoAlgorithm
{
private:
	// Methods
	std::string generatePad(unsigned int length) const;
	std::string xorStrings(const std::string& str1, const std::string& str2) const;

public:

	// Methods
	std::string encrypt(const std::string& message) const override;
	std::string decrypt(const std::string& cipher) const override;
};