#pragma once

#include <vector>
#include <map>
#include "Question.h"

class Game
{
private:
	// fields
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	unsigned int m_gameId;

	// methods
	void submitGameStatusToDB(const GameData&)

public:
	// methods
	void getQuestionsForUser(const LoggedUser& user);
	void submitAnswer();
	void removePlayer();

};