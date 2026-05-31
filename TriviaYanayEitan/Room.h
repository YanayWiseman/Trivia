#pragma once

#include <iostream>
#include <vector>
#include <mutex>
#include "Constants.h"
#include "LoggedUser.h"

struct RoomData
{
	unsigned int id = 0;
	std::string name;
	unsigned int maxPlayers = 0;
	unsigned int numOfQuestionsInGame = 0;
	unsigned int timePerQuestion = 0;
	RoomStatus status = Closed;
};

class Room
{
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
	mutable std::mutex m_mutex;

public:
	Room(const RoomData& data);
	~Room() = default;
	bool isFull();
	RoomData getData() const;
	Status addUser(const LoggedUser& usr);
	void removeUser(const LoggedUser& usr);
	void setStatus(RoomStatus status);
	std::vector<std::string> getAllUsers() const;
	std::vector<LoggedUser> getUsers() const;
};