#include "Room.h"


/*
	Function constructs new Room object.
	Input: data (RoomData)
	Output: None (constructor)
*/

Room::Room(const RoomData& data) : m_metadata(data) {}


/*
	Function returns room data.
	Input: None
	Output: Room data (RoomData)
*/

RoomData Room::getData()
{
	std::lock_guard<std::mutex> lock(this->m_mutex);

	return this->m_metadata;
}


/*
	Function adds user to rooms user vector.
	Input: usr (LoggedUser)
	Output: success or failure
*/

Status Room::addUser(const LoggedUser& usr)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);
	m_users.push_back(usr);
	return Success;
}


/*
	Function removes user from user vector.
	Input: usr (LoggedUser)
	Output: None (void)
*/

void Room::removeUser(const LoggedUser& usr)
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

	return users;
}

/*
	Setter method to se the room status
	input: the new status
	output: no return values
*/

void Room::setStatus(RoomStatus status)
{
	this->m_metadata.status = status;
}