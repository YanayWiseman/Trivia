#include "LoggedUser.h"


/*
	Function constructs LoggedUser object.
	Input: username (const std::string&)
	Output: None (constructor)
*/

LoggedUser::LoggedUser(const std::string& username)
{
	this->m_username = username;
}


/*
	Function returns username of user.
	Input: None
	Output: Username of user (std::string)
*/

const std::string LoggedUser::getUsername() const
{
	return this->m_username;
}
