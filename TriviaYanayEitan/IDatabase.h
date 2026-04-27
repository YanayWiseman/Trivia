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
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
	virtual Status addNewUser(const std::string& username, const std::string& password, const std::string& mail) = 0;
};