#pragma once

#include <string>
#include <iostream>
#include "sqlite3.h"
#include "Constants.h"
#include "IDatabase.h"

#define DB_FILENAME "Trivia.sqlite"

typedef struct UserInfo
{
	std::string username;
	std::string password;
	std::string email;
	std::string address;
	std::string phone;
	std::string birthdate;
} UserInfo;

class SqliteDatabase : public IDatabase
{
private:
	// fields
	sqlite3* m_db;  // data base object
	std::string m_dbFile;  // data base file name

	// methods
	bool executeSqlQuery(const std::string& statement) const;
	sqlite3_stmt* prepare(const char* statement) const;
	static std::string colText(sqlite3_stmt* stmt, int col);

public:
	// C'tor and D'tor
	SqliteDatabase();
	~SqliteDatabase();

	// methods
	bool open() override;
	bool close() override;

	// queries
	bool doesUserExist(const std::string& username) const override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	Status addNewUser(const UserInfo userInfo) override;
};