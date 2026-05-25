#include "GameManager.h"

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

Game createGame(const Room& room)
{

}

/*
	Method to delete a game from the game manager's map
	Input: the game's id
	Output: no return values
*/

void deleteGame(unsigned int gameId)
{

}
