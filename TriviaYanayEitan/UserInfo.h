#pragma once

#include <string>

typedef struct UserInfo
{
	std::string username;
	std::string password;
	std::string email;
	std::string address;
	std::string phone;
	std::string birthdate;
} UserInfo;

typedef struct UserStatistics
{
	std::string username = "";
	int numAnswered = 0;
	int correctAnswers = 0;
	float averageTime = 0.0;
	int gamesCount = 0;
} UserStatistics;