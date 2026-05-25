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

/*
	Setter method to set the players in the game
	input: a vector of logged users, will be transitioned into the map
	output: no return values
*/

void Game::setPlayers(const std::vector<LoggedUser>& players)
{
	// this method will be initializing the player's game data to all 0's
	GameData startingGameData = { this->m_questions[0], 0, 0, 0 };

	for (LoggedUser player : players)
	{
		this->m_players[player] = startingGameData;
	}
}

/*
	Setter method to set the questions for the game
	input: a vector of questions
	output: no return values
*/

void Game::setQuestions(const std::vector<Question>& questions)
{
	this->m_questions = questions;
}


/*
	Getter method to get the game's id
	input: no input values
	output: the game's id (unsigned int)
*/

unsigned int Game::getGameId()
{
	return this->m_gameId;
}
