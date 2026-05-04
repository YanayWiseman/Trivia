#pragma once

#include <string>
#include "Constants.h"
#include "UserInfo.h"

class IDatabase
{
public:
    virtual ~IDatabase() = default;

    virtual bool open() = 0;
    virtual bool close() = 0;

    // USER operations
    virtual bool doesUserExist(const std::string& username) const = 0;
    virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
    virtual Status addNewUser(const UserInfo& userInfo) = 0;

    // STATISTICS operations
    virtual float getPlayerAverageAnswerTime(std::string& username) const = 0;
    virtual int getNumOfCorrectAnswers(std::string& username) const = 0;
    virtual int getNumOfTotalAnswers(std::string& username) const = 0;
    virtual int getNumOfPlayerGames(std::string& username) const = 0;
};