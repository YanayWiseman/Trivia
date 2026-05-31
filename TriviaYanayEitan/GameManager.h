#pragma once

#include <list>
#include <string>
#include <vector>
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
public:
	// methods
	Game& createGame(const Room& room);
	Game& getGame(unsigned int gameId);
	void deleteGame(unsigned int gameId);
	void submitGameStatusToDB(const std::string& username, const GameData& gameData);

	// singleton methods
	static GameManager& getInstance();

	GameManager(const GameManager& other) = delete;
	GameManager& operator=(const GameManager& other) = delete;
};