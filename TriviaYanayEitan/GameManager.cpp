#include "GameManager.h"

/*
	Constructor for the GameManager Object
	input: no input values
	output: no return values
*/

GameManager::GameManager()
{
	this->m_database = &SqliteDatabase::getInstance();
}

/*
	Function creates single instance of GameManager.
	Input: None
	Output: New GameManager instance (GameManager&)
*/

GameManager& GameManager::getInstance()
{
	static GameManager instance;
	return instance;
}

/*
	Method to create a room in the game manager's map
	Input: the room of the game
	Output: the new Game struct created
*/

Game GameManager::createGame(const Room& room)
{
	Game game(room.getData().id);

	game.setPlayers(room.getUsers());
	std::list questionsList = this->m_database->getQuestions(AMOUNT_OF_QUESTIONS);
	std::vector<int> myVector(questionsList.begin(), questionsList.end());
	game.setQuestions();

	this->m_games.push_back(game);
}

/*
	Method to delete a game from the game manager's map
	Input: the game's id
	Output: no return values
*/

void GameManager::deleteGame(unsigned int gameId)
{

}
