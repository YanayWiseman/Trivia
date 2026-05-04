#include "Room.h"


/*
	Function adds user to rooms user vector.
	Input: usr (LoggedUser)
	Output: None (void)
*/

void Room::addUser(LoggedUser usr)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);
	m_users.push_back(usr);
}


/*
	Function removes user from user vector.
	Input: usr (LoggedUser)
	Output: None (void)
*/

void Room::removeUser(LoggedUser usr)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);
	
	for (auto it = this->m_users.begin(); it != this->m_users.end(); ++it)
	{
		if (it->getUsername() == usr.getUsername())
		{
			this->m_users.erase(it);
			return;
		}
	}
}


/*
	Function returns a string vector of all connected usernames.
	Input: None
	Output: Vector of all connected usernames (std::vector<std::string>)
*/

std::vector<std::string> Room::getAllUsers() const
{
	std::lock_guard<std::mutex> lock(this->m_mutex);
	std::vector<std::string> users;

	for (auto it = this->m_users.begin(); it != this->m_users.end(); it++)
	{
		users.push_back(it->getUsername());
	}
}
