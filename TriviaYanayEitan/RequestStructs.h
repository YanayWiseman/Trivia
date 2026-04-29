#pragma once

#include <string>

typedef struct LoginRequest
{
	std::string username;
	std::string password;
} LoginRequest;

typedef struct SignupRequest
{
    std::string username;
    std::string password;
    std::string email;
    std::string address;
    std::string phone;
    std::string birthdate;
} SignupRequest;

