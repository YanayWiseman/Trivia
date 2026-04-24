#pragma once

#include <vector>
#include "IDatabase.h"
#include "LoggedUser.h"
#include "Constants.h"

class LoginManager
{
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

public:
	Status signup(const std::string& username, const std::string& email, const std::string& password);
	Status login(const std::string& username, const std::string& password);
	void logout(const std::string& username);
};