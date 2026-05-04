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
	std::string username;
	int numAnswered;
	int correctAnswers;
	float averageTime;
	int gamesCount;
} UserStatistics;