#pragma once

#include <vector>
#include <list>
#include "IDatabase.h"
#include "SqliteDatabase.h"
#include "Room.h"
#include "Game.h"

#define AMOUNT_OF_QUESTIONS 10

class GameManager
{
private:
	// fields
	IDatabase* m_database;
	std::vector<Game> m_games;

	// methods
	GameManager();

	void submitGameStatusToDB(const LoggedUser& user, const GameData& gameData);

public:
	// methods
	Game createGame(const Room& room);
	void deleteGame(unsigned int gameId);

	// signlton methods
	static GameManager& getInstance();

	GameManager(const GameManager& other) = delete;
	GameManager& operator=(const GameManager& other) = delete;
};