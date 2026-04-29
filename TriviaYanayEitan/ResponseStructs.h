#pragma once

#include <string>

typedef struct LoginResponse
{
	unsigned int status;
} LoginResponse;

typedef struct SignupResponse
{
	unsigned int status;
} SignupResponse;

typedef struct ErrorResponse
{
	std::string error;
} ErrorResponse;
