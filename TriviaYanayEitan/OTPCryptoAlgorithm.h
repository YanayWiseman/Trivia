#pragma once

#include <ctime>
#include "CryptoAlgorithm.h"

class OTPCryptoAlgorithm : public CryptoAlgorithm
{
private:
	// Methods
	std::string generatePad(unsigned int length) const;

public:
	// C'tor
	OTPCryptoAlgorithm();

	// Methods
	std::string encrypt(const std::string& message) const override;
	std::string decrypt(const std::string& cipher) const override;
};