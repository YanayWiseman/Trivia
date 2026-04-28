#pragma once

#include <vector>
#include <regex>
#include "IDatabase.h"
#include "SqliteDataBase.h"
#include "LoggedUser.h"
#include "Constants.h"

class LoginManager
{
private:
	// fields
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

	// methods
	bool isUserInfoValid(const UserInfo& user);
	bool isPasswordValid(const std::string& password);
	bool isEmailValid(const std::string& email);
	bool isAddressValid(const std::string& address);
	bool isPhoneValid(const std::string& phone);
	bool isBirthDateValid(const std::string& birthDate);
public:
	// methods
	LoginManager(IDatabase* database);
	Status signup(const UserInfo& user);
	Status login(const std::string& username, const std::string& password);
	void logout(const std::string& username);
};