#include "Game.h"

/*
	Constructor for the Game object
	input: no input values
	output: no return values
*/

Game::Game(unsigned int gameId, unsigned int timePerQuestion) : m_gameId(gameId), m_timePerQuestion(timePerQuestion)
{
}

/*
	Method to get the questions for a given user
	input: the user
	output: the user's current question
*/

Question Game::getQuestionsForUser(const LoggedUser& user)
{
	auto it = this->m_players.find(user.getUsername());

	if (it == this->m_players.end())
	{
		throw std::runtime_error("error: User not found");
	}

	GameData& gameData = it->second;

	gameData.questionStartTime = std::chrono::steady_clock::now();

	return gameData.currentQuestion;
}

/*
	Method to submit the answer
	input: the user that submits the answer, the answer's id and the answer time 
	output: no return values
*/

void Game::submitAnswer(const LoggedUser& user, unsigned int answerId, float answerTime)
{
	auto it = this->m_players.find(user.getUsername());

	if (it == this->m_players.end())
	{
		throw std::runtime_error("error: User not found");
	}

	GameData& gameData = it->second;

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
	auto it = this->m_players.find(user.getUsername());

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
	if (this->m_questions.empty())
	{
		throw std::runtime_error("error: No questions in game");
	}

	for (int i = 0; i < players.size(); i++)
	{
		GameData gameData =
		{
			std::chrono::steady_clock::now(),
			this->m_questions[0],
			0,
			0,
			0,
			0
		};

		this->m_players.insert(std::pair<std::string, GameData>(players[i].getUsername(), gameData));
	}
}

/*
	Getter method to get the players in the game
	input: no input values
	output: the mao of logged users and their game data
*/

std::map<std::string, GameData> Game::getPlayers()
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


/*
	Function calculates how many seconds passed since the user's current question started
	Input: user (const LoggedUser&)
	Output: answer time in seconds (float)
*/

float Game::getAnswerTime(const LoggedUser& user) const
{
	auto it = this->m_players.find(user.getUsername());

	if (it == this->m_players.end())
	{
		throw std::runtime_error("error: User not found");
	}

	const GameData& gameData = it->second;

	auto now = std::chrono::steady_clock::now();

	float answerTime = std::chrono::duration<float>(now - gameData.questionStartTime).count();

	if (answerTime > this->m_timePerQuestion)
	{
		answerTime = (float)this->m_timePerQuestion;
	}

	return answerTime;
}


/*
	Function calculates how many seconds are left for the user's current question
	Input: user (const LoggedUser&)
	Output: remaining time in seconds (float)
*/

float Game::getRemainingTime(const LoggedUser& user) const
{
	float answerTime = this->getAnswerTime(user);

	if (answerTime >= this->m_timePerQuestion)
	{
		return 0;
	}

	return this->m_timePerQuestion - answerTime;
}


/*
	Function returns the correct answer id of the user's current question
	Input: user (const LoggedUser&)
	Output: correct answer id (unsigned int)
*/

unsigned int Game::getCurrentCorrectAnswerId(const LoggedUser& user) const
{
	auto it = this->m_players.find(user.getUsername());

	if (it == this->m_players.end())
	{
		throw std::runtime_error("error: User not found");
	}

	return it->second.currentQuestion.getCorrectAnswerId();
}


/*
	Function returns the time limit for each question
	Input: None
	Output: time per question in seconds (unsigned int)
*/

unsigned int Game::getTimePerQuestion() const
{
	return this->m_timePerQuestion;
}


