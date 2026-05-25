#pragma once

#include <vector>
#include "IDatabase.h"
#include "Room.h"

class GameManager
{
private:
	// fields
	IDatabase* m_database;
	std::vector<Game> m_games;

public:
	// methods
	Game createGame(const Room& room);
	void deleteGame(unsigned int gameId);
};