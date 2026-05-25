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
	Method to get the questions for a given user
	input: the user
	output: the user's current question
*/

Question Game::getQuestionsForUser(const LoggedUser& user)
{
	return this->m_players[user].currentQuestion;
}

/*
	Method to submit the answer
	input: the user that submits the answer, the answer's id and the answer time 
	output: no return values
*/

void Game::submitAnswer(const LoggedUser& user, unsigned int answerId, float answerTime)
{
	GameData& gameData = this->m_players[user];

	if (answerId == gameData.currentQuestion.getCorrectAnswerId())
	{
		gameData.correctAnswerCount++;
	}
	else
	{
		gameData.wrongAnswerCount++;
	}

	unsigned int totalAnswers = gameData.correctAnswerCount + gameData.wrongAnswerCount;

	// update average answer time
	gameData.averageAnswerTime = ((gameData.averageAnswerTime * (totalAnswers - 1)) + answerTime) / totalAnswers;

	// move to the next question if it exists
	if (gameData.currentQuestionIndex + 1 < this->m_questions.size())
	{
		gameData.currentQuestionIndex++;
		gameData.currentQuestion = this->m_questions[gameData.currentQuestionIndex];
	}
}

/*
	Method to remove a player from the game
	input: the user to remove
	output: no return values
*/

void Game::removePlayer(const LoggedUser& user)
{
	auto it = this->m_players.find(user);

	if (it != this->m_players.end())
	{
		this->m_players.erase(it);
	}
}

/*
	Setter method to set the players in the game
	input: a vector of logged users, will be transitioned into the map
	output: no return values
*/

void Game::setPlayers(const std::vector<LoggedUser>& players)
{
	// this method will be initializing the player's game data to all 0's
	GameData startingGameData = { this->m_questions[0], 0, 0, 0, 0 };

	for (LoggedUser player : players)
	{
		this->m_players[player] = startingGameData;
	}
}

/*
	Getter method to get the players in the game
	input: no input values
	output: the mao of logged users and their game data
*/

std::map<LoggedUser, GameData> Game::getPlayers()
{
	return this->m_players;
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

