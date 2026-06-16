#pragma once

#include <memory>
#include <mutex>
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
	mutable std::mutex m_mutex;
	IDatabase* m_database;
	std::vector<std::unique_ptr<Game>> m_games;

	// methods
	GameManager();
public:
	// methods
	Game& createGame(const Room& room, const std::vector<Question>& questionsToAdd);
	Game& getGame(unsigned int gameId);
	void deleteGame(unsigned int gameId);
	void submitGameStatusToDB(const std::string& username, const GameData& gameData);

	// singleton methods
	static GameManager& getInstance();

	GameManager(const GameManager& other) = delete;
	GameManager& operator=(const GameManager& other) = delete;
};