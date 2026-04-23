#pragma once

#include <string>
#include "sqlite3.h"
#include "IDataBase.h"

#define DB_FILENAME "Trivia.sqlite"

class SqliteDataBase : public IDataBase
{
private:
	// fields
	sqlite3* m_db;  // data base object
	std::string m_dbFile;  // data base file name

public:
	// C'tor and D'tor
	SqliteDataBase();
	~SqliteDataBase();;

	// methods
	bool open() override;
	bool close() override;

	// queries
	int doesUserExist(const std::string& username) const override;
	int doesPasswordMatch(const std::string& password1, const std::string& password2) const override;
	int addNewUser(const std::string& username, const std::string& password, const std::string& mail) const override;
};