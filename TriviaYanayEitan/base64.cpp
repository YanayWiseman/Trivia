#include "Base64.h"

static const char BASE64_TABLE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
	Function encodes binary data into Base64 text.
	Input: input (const std::string&)
	Output: The Base64 encoded string (std::string)
*/

std::string Base64::encode(const std::string& input)
{
	std::string output;
	int val = 0;
	int valb = BASE64_INITIAL_BIT_OFFSET;

	int i = 0;
	for (i = 0; i < input.size(); i++)
	{
		unsigned char c = static_cast<unsigned char>(input[i]);

		val = (val << BYTE_BITS) + c;
		valb += BYTE_BITS;

		while (valb >= 0)
		{
			output.push_back(BASE64_TABLE[(val >> valb) & BASE64_MASK]);
			valb -= BASE64_BITS;
		}
	}

	if (valb > BASE64_INITIAL_BIT_OFFSET)
	{
		output.push_back(BASE64_TABLE[((val << BYTE_BITS) >> (valb + BYTE_BITS)) & BASE64_MASK]);
	}

	while (output.size() % BASE64_OUTPUT_BLOCK_SIZE)
	{
		output.push_back(BASE64_PADDING_CHAR);
	}

	return output;
}