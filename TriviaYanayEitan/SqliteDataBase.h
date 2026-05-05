#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include <algorithm>
#include <list>
#include "sqlite3.h"
#include "Constants.h"
#include "IDatabase.h"
#include "UserInfo.h"
#include "Question.h"

#define DB_FILENAME "Trivia.sqlite"

#define QUESTION_ID_COL 0
#define QUESTION_TEXT_COL 1

#define ANSWER_TEXT_COL 0
#define IS_CORRECT_COL 1

class SqliteDatabase : public IDatabase
{
private:
	// fields
	sqlite3* m_db;  // data base object
	std::string m_dbFile;  // data base file name
	mutable std::mutex m_dbMutex;

	// methods
	SqliteDatabase();

	bool executeSqlQuery(const std::string& statement) const;
	sqlite3_stmt* prepare(const char* statement) const;
	static std::string colText(sqlite3_stmt* stmt, int col);
	float getStatisticValue(const std::string& username, const std::string& column) const;

public:
	// D'tor
	~SqliteDatabase();

	// methods
	bool open() override;
	bool close() override;

	static SqliteDatabase& getInstance();

	SqliteDatabase(const SqliteDatabase& other) = delete;
	SqliteDatabase& operator=(const SqliteDatabase& other) = delete;

	// USER operations
	bool doesUserExist(const std::string& username) const override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	Status addNewUser(const UserInfo& userInfo) override;

	// QUESTIONS operations
	std::list<Question> getQuestions(int amount);


	// STATISTICS operations
	float getPlayerAverageAnswerTime(const std::string& username) const override;
	int getNumOfCorrectAnswers(const std::string& username) const override;
	int getNumOfTotalAnswers(const std::string& username) const override;
	int getNumOfPlayerGames(const std::string& username) const override;
	std::vector<std::pair<std::string, int>> getHighScores() const override;
};