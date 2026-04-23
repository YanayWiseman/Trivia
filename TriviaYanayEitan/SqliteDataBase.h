#pragma once

#include <string>
#include <iostream>
#include "sqlite3.h"
#include "IDatabase.h"

#define DB_FILENAME "Trivia.sqlite"

class SqliteDatabase : public IDatabase
{
private:
	// fields
	sqlite3* m_db;  // data base object
	std::string m_dbFile;  // data base file name

	// methods
	bool executeSqlQuery(const std::string& statement) const;
	sqlite3_stmt* prepare(const char* statement) const;

public:
	// C'tor and D'tor
	SqliteDatabase();
	~SqliteDatabase();

	// methods
	bool open() override;
	bool close() override;

	// queries
	bool doesUserExist(const std::string& username) const override;
	bool doesPasswordMatch(const std::string& password1, const std::string& password2) const override;
	void addNewUser(const std::string& username, const std::string& password, const std::string& mail) const override;
};