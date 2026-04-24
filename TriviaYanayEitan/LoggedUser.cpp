#include "LoggedUser.h"

LoggedUser::LoggedUser(std::string username)
{
	this->m_username = username;
}

const std::string LoggedUser::getUsername() const
{
	return this->m_username;
}
