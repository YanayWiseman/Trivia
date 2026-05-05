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
    virtual float getPlayerAverageAnswerTime(const std::string& username) const = 0;
    virtual int getNumOfCorrectAnswers(const std::string& username) const = 0;
    virtual int getNumOfTotalAnswers(const std::string& username) const = 0;
    virtual int getNumOfPlayerGames(const std::string& username) const = 0;
    virtual int getPlayerScore(const std::string& username) const = 0;
    virtual std::vector<std::pair<std::string, int>> getHighScores() const = 0;
};