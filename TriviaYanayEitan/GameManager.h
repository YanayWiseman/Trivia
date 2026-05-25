#pragma once

#include <vector>
#include "IDatabase.h"
#include "Room.h"
#include "Game.h"

class GameManager
{
private:
	// fields
	IDatabase* m_database;
	std::vector<Game> m_games;

	// methods
	GameManager() = default;

public:
	// methods
	Game createGame(const Room& room);
	void deleteGame(unsigned int gameId);

	// signlton methods
	static GameManager& getInstance();

	GameManager(const GameManager& other) = delete;
	GameManager& operator=(const GameManager& other) = delete;
};