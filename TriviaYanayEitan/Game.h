#pragma once

#include <vector>
#include <map>
#include "Question.h"
#include "LoggedUser.h"

struct GameData
{
	Question currentQuestion;
	unsigned int currentQuestionIndex;
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
	Question getQuestionsForUser(const LoggedUser& user);
	void submitAnswer(const LoggedUser& user, unsigned int answerId, float answerTime);
	void removePlayer(const LoggedUser& user);
	void setPlayers(const std::vector<LoggedUser>& players);
	void setQuestions(const std::vector<Question>& questions);
	unsigned int getGameId();
};