#pragma once

#include <string>

class IDataBase
{
public:
	// methods
	virtual bool open();
	virtual bool close();

	// queries
	virtual int doesUserExist(const std::string& username) const = 0;
	virtual int doesPasswordMatch(const std::string& password1, const std::string& password2) const = 0;
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& mail) const = 0;
};