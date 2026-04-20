#pragma once

#include <string>

typedef struct LoginResopnse
{
	unsigned int status;
} LoginResponse;

typedef struct SignupResopnse
{
	unsigned int status;
} SignupResponse;

typedef struct ErrorResopnse
{
	std::string error;
} ErrorResponse;
