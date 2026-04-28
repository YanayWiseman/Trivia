#include "SqliteDatabase.h"

/*
	Constructor for the SqliteDatabase class, to contsruct the object
	input: no input values
	output: no return values
*/

SqliteDatabase::SqliteDatabase()
	: m_db(nullptr), m_dbFile(DB_FILENAME)
{
	// in order to execute queries on the database we need to open it
	if (!open())
	{
		throw std::runtime_error("failed opening db");
	}

	// now we can create the tables
	executeSqlQuery("CREATE TABLE IF NOT EXISTS USER ("
	" USERNAME TEXT PRIMARY KEY,"
	" PASSWORD TEXT NOT NULL,"
	" EMAIL TEXT NOT NULL," 
	" ADDRESS TEXT NOT NULL,"
	" PHONE TEXT NOT NULL,"
	" BIRTHDATE TEXT NOT NULL);");
}

/*
	Destructor for the SqliteDatabase class, to destruct the object
	input: no input values
	output: no return values
*/

SqliteDatabase::~SqliteDatabase()
{
	// just close the database
	close();
}

/*
	Method to open the database
	input: no input values
	output: bool, true if opened and false otherwise
*/

bool SqliteDatabase::open()
{
	// safety check
	// just in case open() has been called already
	if (this->m_db != nullptr)
	{
		return true;
	}

	// open the data base
	int res = sqlite3_open(this->m_dbFile.c_str(), &this->m_db);

	// check if opened
	if (res != SQLITE_OK)
	{
		std::cout << sqlite3_errmsg(this->m_db);
		this->m_db = nullptr;
		return false;
	}
	return true;
}

/*
	Method to close the database
	input: no input values
	output: no return values
*/

bool SqliteDatabase::close()
{
	// if the database is open
	if (this->m_db != nullptr)
	{
		bool ok = sqlite3_close(this->m_db) == SQLITE_OK;
		this->m_db = nullptr;

		return ok;
	}

	return true;
}

/*
	Method to check if a given user exists in database
	input: the username to check
	output: true if user exists in database and false otherwise
*/

bool SqliteDatabase::doesUserExist(const std::string& username) const
{
	sqlite3_stmt* stmt = prepare("SELECT COUNT(*) FROM USER WHERE USERNAME = ?;");
	if (!stmt)
	{
		return false;
	}

	bool exists = false;
	// bind the statement
	sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		// check if the count is bigger then 0
		exists = (sqlite3_column_int(stmt, 0) > 0);
	}

	sqlite3_finalize(stmt);

	return exists;
}

/*
	Method to check if a given password matches its username
	input: the username and given password
	output: true if password matches and false otherwise
*/

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	sqlite3_stmt* stmt = prepare("SELECT PASSWORD FROM USER WHERE USERNAME = ?;");
	if (!stmt)
	{
		return false;
	}

	// bind the username
	sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

	bool match = false;

	// should be only one row
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		// check if it matches the given password
		match = SqliteDatabase::colText(stmt, 0) == password;
	}

	sqlite3_finalize(stmt);

	return match;
}

/*
	Method to add a new user to the database
	input: the user's fields
	output: no return values
*/

Status SqliteDatabase::addNewUser(const UserInfo userInfo)
{
	sqlite3_stmt* stmt = prepare("INSERT INTO USER (USERNAME, PASSWORD, EMAIL, ADDRESS, PHONE, BIRTHDATE) "
		"VALUES (?, ?, ?, ?, ?, ?);"
	);
	if (!stmt)
	{
		return Failiure;
	}
	// bind the values
	sqlite3_bind_text(stmt, 1, userInfo.username.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, userInfo.password.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, userInfo.email.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 4, userInfo.address.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 5, userInfo.phone.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 6, userInfo.birthdate.c_str(), -1, SQLITE_TRANSIENT);

	// step
	if (sqlite3_step(stmt) != SQLITE_DONE)
	{
		std::cout << "Failed adding a new user: " << userInfo.username << " - " << userInfo.email <<  std::endl;
		return Failiure;
	}

	sqlite3_finalize(stmt);
	return Success;
}

/*
	Helper static method to convert the const unsigned char* that sqlite3_colums_text() returns
	input: the statement and the column to convert
	output: the converted string
*/

std::string SqliteDatabase::colText(sqlite3_stmt* stmt, int col)
{
	// extract it from the records
	const unsigned char* text = sqlite3_column_text(stmt, col);

	if (text)
	{
		// first of all, sqlite3_column_bytes returns the number of bytes in the text column
		// and this string constructor copies the first text + sqlite3_column_bytes(stmt, col)
		// from into the text "array"
		return std::string(text, text + sqlite3_column_bytes(stmt, col));
	}
	else
	{
		return std::string();
	}
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