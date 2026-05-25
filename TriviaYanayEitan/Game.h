#pragma once

#include <vector>
#include <map>
#include "Question.h"
#include "LoggedUser.h"

struct GameData
{
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	float averageAnswerTime;
};

class Game
{
private:
	// fields
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	unsigned int m_gameId;

	// methods
	void submitGameStatusToDB(const LoggedUser& user, const GameData& gameData);

public:
	// c'tor
	Game(unsigned int gameId);
	~Game();
	// methods
	void getQuestionsForUser(const LoggedUser& user);
	void submitAnswer();
	void removePlayer();
	void setPlayers(const std::vector<LoggedUser>& players);
	void setQuestions(const std::vector<Question>& questions);
	unsigned int getGameId();
};