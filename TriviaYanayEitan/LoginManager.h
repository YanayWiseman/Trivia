#pragma once

#include <vector>
#include <regex>
#include <mutex>
#include "IDatabase.h"
#include "LoggedUser.h"
#include "Constants.h"
#include "UserInfo.h"

class LoginManager
{
private:
	// fields
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
	std::mutex m_mutex;

	// methods
	LoginManager(IDatabase* database);

	bool isUserInfoValid(const UserInfo& user);
	bool isPasswordValid(const std::string& password);
	bool isEmailValid(const std::string& email);
	bool isAddressValid(const std::string& address);
	bool isPhoneValid(const std::string& phone);
	bool isBirthDateValid(const std::string& birthDate);
public:
	// methods
	Status signup(const UserInfo& user);
	Status login(const std::string& username, const std::string& password);
	void logout(const std::string& username);

	static LoginManager& getInstance(IDatabase* database);

	LoginManager(const LoginManager& other) = delete;
	LoginManager& operator=(const LoginManager& other) = delete;
};