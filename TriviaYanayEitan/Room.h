#pragma once

#include <iostream>
#include <vector>
#include <mutex>
#include "Constants.h"
#include "LoggedUser.h"

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	RoomStatus status;
};

class Room
{
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
	mutable std::mutex m_mutex;

public:
	void addUser(LoggedUser usr);
	void removeUser(LoggedUser usr);
	std::vector<std::string> getAllUsers() const;
};