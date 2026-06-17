#pragma once

#include <string>

#define BYTE_BITS 8
#define BASE64_BITS 6
#define BASE64_MASK 0x3F
#define BASE64_INITIAL_BIT_OFFSET -6
#define BASE64_OUTPUT_BLOCK_SIZE 4
#define BASE64_PADDING_CHAR '='


class Base64
{
public:
	static std::string encode(const std::string& input);
};