#include "OTPCryptoAlgorithm.h"

/*
	Method encrypts a given message using OTP (One Time Pad)
	input: The message to encrypt
	output: The encrypted message
*/

std::string OTPCryptoAlgorithm::encrypt(const std::string& message) const
{
	// randomly generate a pad with the same length of the message
	std::string pad = generatePad(message.length());

	// store the key in database  (key id is the current timestamp as string)
	std::string keyId = "otp_" + std::to_string(time(nullptr));
	SqliteDatabase::getInstance().storeKey(keyId, pad);

	// xor the message with the pad
	std::string cipherText = xorStrings(message, pad);

	// embed the keyId length + keyId in front of the ciphertext
	// format: [4 bytes: keyId length][keyId bytes][ciphertext bytes]
	uint32_t idLen = static_cast<uint32_t>(keyId.size());
	std::string result(4 + idLen + cipherText.size(), '\0');

	// add the keyid length
	result[0] = static_cast<char>((idLen >> 24) & 0xFF);
	result[1] = static_cast<char>((idLen >> 16) & 0xFF);
	result[2] = static_cast<char>((idLen >> 8) & 0xFF);
	result[3] = static_cast<char>((idLen) & 0xFF);

	// add the key id
	for (uint32_t i = 0; i < idLen; i++)
	{
		result[4 + i] = keyId[i];
	}

	// add the cipher text
	size_t i = 0;
	for (i = 0; i < cipherText.size(); i++)
	{
		result[4 + idLen + i] = cipherText[i];
	}

	return result;
}

/*
	Method decrypts a given cipher according to OTP (One Time Pad)
	input: The cipher to decrypt
	output: The decrypted message
*/

std::string OTPCryptoAlgorithm::decrypt(const std::string& message) const
{
	if (message.size() < 4)
	{
		throw std::runtime_error("OTP decrypt: message too short");
	}

	// parse the keyId length from the first 4 bytes
	uint32_t idLen =
		(static_cast<uint8_t>(message[0]) << 24) |
		(static_cast<uint8_t>(message[1]) << 16) |
		(static_cast<uint8_t>(message[2]) << 8) |
		(static_cast<uint8_t>(message[3]));

	if (message.size() < 4 + idLen)
	{
		throw std::runtime_error("OTP decrypt: message truncated");
	}

	// extract the keyId and look up the pad from the fatabase
	std::string keyId = message.substr(4, idLen);
	std::string cipherText = message.substr(4 + idLen);

	std::string pad = SqliteDatabase::getInstance().loadKey(keyId);
	if (pad.empty())
	{
		throw std::runtime_error("OTP decrypt: key not found in DB for id: " + keyId);
	}

	// xor ciphertext with the pad to recover the original message
	return xorStrings(cipherText, pad);
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
	// We can allocate the exact memory since we know the length of the result
	std::string result(str1.length(), '\0');
	int i = 0;

	for (i = 0; i < str1.length(); i++)  // str1.length() == str2.length() gurenteed
	{
		result[i] = str1[i] ^ str2[i];  // ^ is xor
	}

	return result;
}
