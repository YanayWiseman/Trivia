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
	std::map<std::string, GameData> m_players;
	unsigned int m_gameId;

public:
	// c'tor
	Game(unsigned int gameId);
	~Game() = default;
	// methods
	Question getQuestionsForUser(const LoggedUser& user);
	void submitAnswer(const LoggedUser& user, unsigned int answerId, float answerTime);
	void removePlayer(const LoggedUser& user);
	void setPlayers(const std::vector<LoggedUser>& players);
	void setQuestions(const std::vector<Question>& questions);
	unsigned int getGameId();
	std::map<std::string, GameData> getPlayers();
};