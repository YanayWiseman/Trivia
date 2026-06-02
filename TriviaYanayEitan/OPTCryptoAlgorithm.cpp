#include "OTPCryptoAlgorithm.h"

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
	return "";
}