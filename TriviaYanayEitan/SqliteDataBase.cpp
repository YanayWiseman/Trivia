#include "SqliteDataBase.h"

/*
	Constructor for the SqliteDataBase class, to contsruct the object
	input: no input values
	output: no return values
*/

SqliteDataBase::SqliteDataBase()
	: m_db(nullptr), m_dbFile(DB_FILENAME)
{

}

/*
	Destructor for the SqliteDataBase class, to destruct the object
	input: no input values
	output: no return values
*/

SqliteDataBase::~SqliteDataBase()
{

}

/*
	Method to open the database
	input: no input values
	output: bool, true if opened and false otherwise
*/

bool SqliteDataBase::open()
{

}

/*
	Method to close the database
	input: no input values
	output: no return values
*/

bool SqliteDataBase::close()
{

}

/*
	Method to check if a given user exists in database
	input: the username to check
	output: true if user exists in database and false otherwise
*/

bool SqliteDataBase::doesUserExist(const std::string& username) const
{

}

/*
	Method to check if two given passwords match
	input: the two passwords
	output: true if passwords match and false otherwise
*/

bool SqliteDataBase::doesPasswordMatch(const std::string& password1, const std::string& password2) const
{

}

/*
	Method to add a new user to the database
	input: the user's name, password and mail
	output: no return values
*/

void SqliteDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& mail) const
{

}