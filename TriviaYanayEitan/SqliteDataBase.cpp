#include "SqliteDataBase.h"

/*
	Constructor for the SqliteDatabase class, to contsruct the object
	input: no input values
	output: no return values
*/

SqliteDatabase::SqliteDatabase()
	: m_db(nullptr), m_dbFile(DB_FILENAME)
{
	// in order to execute queries on the database we need to open it
	open();
}

/*
	Destructor for the SqliteDatabase class, to destruct the object
	input: no input values
	output: no return values
*/

SqliteDatabase::~SqliteDatabase()
{
	close();
}

/*
	Method to open the database
	input: no input values
	output: bool, true if opened and false otherwise
*/

bool SqliteDatabase::open()
{

}

/*
	Method to close the database
	input: no input values
	output: no return values
*/

bool SqliteDatabase::close()
{

}

/*
	Method to check if a given user exists in database
	input: the username to check
	output: true if user exists in database and false otherwise
*/

bool SqliteDatabase::doesUserExist(const std::string& username) const
{

}

/*
	Method to check if two given passwords match
	input: the two passwords
	output: true if passwords match and false otherwise
*/

bool SqliteDatabase::doesPasswordMatch(const std::string& password1, const std::string& password2) const
{

}

/*
	Method to add a new user to the database
	input: the user's name, password and mail
	output: no return values
*/

void SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& mail) const
{

}

/*
	Helper method to exectue a given SQL query statement to avoid code duplication 
	and implement the encapsulation pricple
	input: the SQL statement as a string
	output: true if statement succeeded and false otherwise (according to SQLITE_OK)
*/

bool SqliteDatabase::executeSqlQuery(const std::string& statement) const
{
	char* errorMsg = nullptr;

	int res = sqlite3_exec(this->m_db, statement.c_str(), nullptr, nullptr, &errorMsg);
	if (res != SQLITE_OK)
	{
		std::cout << "Error:" << errorMsg << std::endl;

		// if sqlite3 used errorMsg we need to free the memory using their functions
		if (errorMsg)
		{
			sqlite3_free(errorMsg);
		}
		return false;
	}

	return true;
}

/*
	Helper method to prepare a given SQL statement
	input: the statement in char form
	output: the sqlite3_stmt*
*/

sqlite3_stmt* SqliteDatabase::prepare(const char* statement) const
{
	sqlite3_stmt* stmt = nullptr;

	// prepare the statement
	int res = sqlite3_prepare_v2(this->m_db, statement, -1, &stmt, nullptr);

	// check for failure
	if (res != SQLITE_OK)
	{
		std::cout << "Error: " << sqlite3_errmsg(this->m_db);
		return nullptr;
	}
	return stmt;
}