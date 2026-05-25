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
