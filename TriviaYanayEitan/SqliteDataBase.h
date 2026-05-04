#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include <algorithm>
#include "sqlite3.h"
#include "Constants.h"
#include "IDatabase.h"
#include "UserInfo.h"

#define DB_FILENAME "Trivia.sqlite"

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

	// STATISTICS operations
	float getPlayerAverageAnswerTime(const std::string& username) const override;
	int getNumOfCorrectAnswers(const std::string& username) const override;
	int getNumOfTotalAnswers(const std::string& username) const override;
	int getNumOfPlayerGames(const std::string& username) const override;
	std::vector<std::pair<std::string, int>> getHighScores() const override;
};