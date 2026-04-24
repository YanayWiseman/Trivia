#pragma once

#include <string>
#include "Constants.h"


class IDatabase
{
public:
	// methods
	virtual ~IDatabase() = default;
	virtual bool open();
	virtual bool close();

	// queries
	virtual bool doesUserExist(const std::string& username) const = 0;
	virtual Status doesPasswordMatch(const std::string& password1, const std::string& password2) const = 0;
	virtual Status addNewUser(const std::string& username, const std::string& password, const std::string& mail) const = 0;
};