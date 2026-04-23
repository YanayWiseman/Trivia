#pragma once

#include <string>
#include "sqlite3.h"
#include "IDataBase.h"

class SqliteDataBase : public IDataBase
{
public:
	// methods
	bool open();
	bool close();

	// queries
	int doesUserExist(const std::string& username) const;
	int doesPasswordMatch(const std::string& password1, const std::string& password2) const;
	int addNewUser(const std::string& username, const std::string& password, const std::string& mail) const;
};