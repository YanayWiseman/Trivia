#pragma once

#include <vector>
#include "IDatabase.h"
#include "SqliteDataBase.h"
#include "LoggedUser.h"
#include "Constants.h"

class LoginManager
{
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

public:
	LoginManager(IDatabase* database);
	Status signup(const UserInfo& user);
	Status login(const std::string& username, const std::string& password);
	void logout(const std::string& username);
};