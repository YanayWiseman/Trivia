#include "Game.h"

/*
	Constructor for the Game object
	input: no input values
	output: no return values
*/

Game::Game(unsigned int gameId)
	: m_gameId(gameId)
{
}

/*
	Private Method to submit the game status to the database
	input: the game's data
	output: no return values
*/

void Game::submitGameStatusToDB(const GameData& gameData)
{

}

/*
	Method to get the questions for a given user
	input: the user
	output: no return values
*/

void Game::getQuestionsForUser(const LoggedUser& user)
{

}

/*
	Method to submit the answer
	input: no input values
	output: no return values
*/

void Game::submitAnswer()
{

}

/*
	Method to remove a player from the game
	input: no input values;
	output: no return values
*/

void Game::removePlayer()
{

}