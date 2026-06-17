#pragma once

#include <vector>
#include <regex>
#include <mutex>
#include "IDatabase.h"
#include "LoggedUser.h"
#include "SqliteDatabase.h"
#include "Constants.h"
#include "UserInfo.h"
#include "OTPCryptoAlgorithm.h"

const unsigned int OTP_PAD_SIZE = 1024 * 1024;

struct SignupResult
{
	Status status;
	std::string clientToServerKeyId;
	std::string serverToClientKeyId;
	std::string clientToServerPad;
	std::string serverToClientPad;
};

class LoginManager
{
private:
	// fields
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
	std::mutex m_mutex;

	// methods
	LoginManager();

	bool isUserInfoValid(const UserInfo& user);
	bool isPasswordValid(const std::string& password);
	bool isEmailValid(const std::string& email);
	bool isAddressValid(const std::string& address);
	bool isPhoneValid(const std::string& phone);
	bool isBirthDateValid(const std::string& birthDate);
public:
	// methods
	~LoginManager() = default;
	SignupResult signup(const UserInfo& user);
	Status login(const std::string& username, const std::string& password);
	void logout(const std::string& username);

	static LoginManager& getInstance();

	LoginManager(const LoginManager& other) = delete;
	LoginManager& operator=(const LoginManager& other) = delete;
};