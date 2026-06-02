#include "OTPCryptoAlgorithm.h"

/*
	Constructor for the OTPCryptoAlgorithm object
	input: no input values
	output: no return values
*/

OTPCryptoAlgorithm::OTPCryptoAlgorithm()
{
	// initialize the time seed
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

/*
	Method encrypts a given message using OTP (One Time Pad)
	input: The message to encrypt
	output: The encrypted message
*/

std::string OTPCryptoAlgorithm::encrypt(const std::string& message) const
{
	
}

/*
	Method decrypts a given cipher according to OTP (One Time Pad)
	input: The cipher to decrypt
	output: The decrypted message
*/

std::string OTPCryptoAlgorithm::decrypt(const std::string& message) const
{
	return "";
}

/*
	Helper method generates a pad to encrypt using OPT
	input: The length of the message (also the length of the pad that will be generated)
	output: The randomly generated pad
*/

std::string OTPCryptoAlgorithm::generatePad(unsigned int length) const
{
	// A pad is a bunch of randomly generated text the same length of the text we want to encrypt
	std::string pad = "";
	int i = 0;

	for (i = 0; i < length; i++)
	{
		char currentChar = static_cast<char>()
	}

	return pad;
}