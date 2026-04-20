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

typedef struct ErrorRespones
{
	std::string error;
} ErrorResponse;
