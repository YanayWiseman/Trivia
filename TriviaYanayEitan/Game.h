#pragma once

#include <vector>
#include <map>
#include <chrono>
#include "Question.h"
#include "LoggedUser.h"

struct GameData
{
	std::chrono::steady_clock::time_point questionStartTime;
	Question currentQuestion;
	unsigned int currentQuestionIndex;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	float averageAnswerTime;
	bool finished;
};

class Game
{
private:
	// fields
	std::vector<Question> m_questions;
	std::map<std::string, GameData> m_players;
	unsigned int m_gameId;
	unsigned int m_timePerQuestion;

public:
	// c'tor
	Game(unsigned int gameId, unsigned int timePerQuestion);
	~Game() = default;

	// methods
	bool hasGameEnded() const;
	Question getQuestionsForUser(const LoggedUser& user);
	void submitAnswer(const LoggedUser& user, unsigned int answerId, float answerTime);
	void removePlayer(const LoggedUser& user);

	// setters
	void setPlayers(const std::vector<LoggedUser>& players);
	void setQuestions(const std::vector<Question>& questions);
	void addQuestion(const Question& question);

	// getters
	unsigned int getGameId() const;
	GameData getGameDataForUser(const LoggedUser& user) const;
	std::map<std::string, GameData> getPlayers();
	float getAnswerTime(const LoggedUser& user) const;
	float getRemainingTime(const LoggedUser& user) const;
	unsigned int getCurrentCorrectAnswerId(const LoggedUser& user) const;
	unsigned int getTimePerQuestion() const;
};