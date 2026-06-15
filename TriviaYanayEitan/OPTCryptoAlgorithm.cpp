#include "OTPCryptoAlgorithm.h"
#include "SqliteDataBase.h"
std::mutex OTPCryptoAlgorithm::m_mutex;

/*
	Function encrypts a given message using OTP pad from the database.
	Input: message (const std::string&), keyId (const std::string&)
	Output: The encrypted message
*/

std::string OTPCryptoAlgorithm::encrypt(const std::string& message, const std::string& keyId) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string pad = SqliteDatabase::getInstance().loadKey(keyId);
	unsigned int offset = SqliteDatabase::getInstance().loadOffset(keyId);

	if (pad.empty())
	{
		throw std::runtime_error("OTP encrypt: key not found: " + keyId);
	}


	// Pad has a finite length, so it may be exhausted eventually.

	if (offset + message.length() > pad.length())
	{
		throw std::runtime_error("OTP encrypt: pad exhausted");
	}


	// Create a new string for our cipher text and fill it with our message, XORed with the pad.

	std::string cipherText;
	cipherText.resize(message.length());

	int i = 0;
	for (i = 0; i < message.length(); i++)
	{
		cipherText[i] = message[i] ^ pad[offset + i];
	}

	SqliteDatabase::getInstance().storeOffset(keyId, offset + static_cast<unsigned int>(message.length()));

	return buildPacket(offset, cipherText);
}


/*
	Function decrypts a given OTP packet.
	Input: packet (const std::string&), keyId (const std::string&)
	Output: The decrypted message
*/

std::string OTPCryptoAlgorithm::decrypt(const std::string& packet, const std::string& keyId) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	if (packet.length() < MIN_LEN)
	{
		throw std::runtime_error("OTP decrypt: packet too short");
	}

	unsigned int offset = readUInt32(packet, OFFEST_INDX);
	unsigned int cipherLen = readUInt32(packet, CIPHER_LEN_INDX);

	if (packet.length() != MIN_LEN + cipherLen)
	{
		throw std::runtime_error("OTP decrypt: packet size mismatch");
	}


	std::string pad = SqliteDatabase::getInstance().loadKey(keyId);

	if (pad.empty())
	{
		throw std::runtime_error("OTP decrypt: key not found: " + keyId);
	}

	if (offset + cipherLen > pad.length())
	{
		throw std::runtime_error("OTP decrypt: pad exhausted or invalid offset");
	}

	unsigned int expectedOffset = SqliteDatabase::getInstance().loadOffset(keyId);
	if (offset != expectedOffset)
	{
		throw std::runtime_error("OTP decrypt: unexpected offset. Expected " + std::to_string(expectedOffset) + ", received " + std::to_string(offset));
	}

	std::string plainText(cipherLen, '\0');

	int i = 0;
	for (i = 0; i < cipherLen; i++)
	{
		plainText[i] = packet[MIN_LEN + i] ^ pad[offset + i];
	}

	SqliteDatabase::getInstance().storeOffset(keyId, offset + cipherLen);

	return plainText;
}


/*
	Helper method generates a pad to encrypt using OPT
	input: The length of the message (also the length of the pad that will be generated)
	output: The randomly generated pad
*/

std::string OTPCryptoAlgorithm::generatePad(unsigned int length) const
{
	std::random_device rnd;
	std::string pad(length, '\0');

	int i = 0;
	for (i = 0; i < length; i++)
	{
		// Takes only 8 bits of the random number.
		pad[i] = static_cast<char>(rnd() & 0xFF);
	}

	return pad;
}


/*
	Function builds an OTP encrypted packet.
	Input: offset (unsigned int), cipherText (const std::string&)
	Output: The full encrypted packet (std::string)
*/

std::string OTPCryptoAlgorithm::buildPacket(unsigned int offset, const std::string& cipherText) const
{
	std::string packet(MIN_LEN + cipherText.length(), '\0');

	writeUInt32(packet, 0, offset);
	writeUInt32(packet, 4, static_cast<unsigned int>(cipherText.length()));

	int i = 0;
	for (i = 0; i < cipherText.length(); i++)
	{
		packet[MIN_LEN + i] = cipherText[i];
	}

	return packet;
}


/*
	Function writes an unsigned integer into a string in big-endian format.
	Input: buffer (std::string&),  index (int), value (unsigned int)
	Output: None (void)
*/

void OTPCryptoAlgorithm::writeUInt32(std::string& buffer, int index, unsigned int value) const
{
	buffer[index] = static_cast<char>((value >> 24) & 0xFF);
	buffer[index + 1] = static_cast<char>((value >> 16) & 0xFF);
	buffer[index + 2] = static_cast<char>((value >> 8) & 0xFF);
	buffer[index + 3] = static_cast<char>(value & 0xFF);
}


/*
	Function reads an unsigned integer from a string in big-endian format.
	Input: buffer (const std::string&), index (int)
	Output: The unsigned integer read from the buffer.
*/

unsigned int OTPCryptoAlgorithm::readUInt32(const std::string& buffer, int index) const
{
	return
		(static_cast<unsigned char>(buffer[index]) << 24) |
		(static_cast<unsigned char>(buffer[index + 1]) << 16) |
		(static_cast<unsigned char>(buffer[index + 2]) << 8) |
		(static_cast<unsigned char>(buffer[index + 3]));
}