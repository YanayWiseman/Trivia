#pragma once

#include <string>

class IDatabase
{
public:
	// d'tor
	virtual ~IDatabase() = default;
	// methods
	virtual bool open() = 0;
	virtual bool close() = 0;

	// queries
	virtual bool doesUserExist(const std::string& username) const = 0;
	virtual bool doesPasswordMatch(const std::string& password1, const std::string& password2) const = 0;
	virtual void addNewUser(const std::string& username, const std::string& password, const std::string& mail) const = 0;
};