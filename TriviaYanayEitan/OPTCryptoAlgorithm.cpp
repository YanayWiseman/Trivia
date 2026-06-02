#include "OTPCryptoAlgorithm.h"

/*
	Constructor for the OTPCryptoAlgorithm object
	input: no input values
	output: no return values
*/

OTPCryptoAlgorithm::OTPCryptoAlgorithm()
{
}

/*
	Method encrypts a given message using OTP (One Time Pad)
	input: The message to encrypt
	output: The encrypted message
*/

std::string OTPCryptoAlgorithm::encrypt(const std::string& message) const
{
	std::string pad = generatePad(message.size());


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
	std::random_device rnd;
	std::mt19937 generator(rnd());
	std::uniform_int_distribution<int> distribution(MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII);

	std::string pad = "";
	int i = 0;

	for (i = 0; i < length; ++i)
	{
		pad += static_cast<char>(distribution(generator));
	}

	return pad;
}

/*
	Helper method applies xor operation to two strings creating a new one
	input: The two strings to xor
	output: the xored string created from both strings
*/

std::string OTPCryptoAlgorithm::xorStrings(const std::string & str1, const std::string & str2) const
{
	std::string result;

	return result;
}
