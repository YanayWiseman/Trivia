#pragma once

#include <string>
#include <list>
#include "Constants.h"
#include "UserInfo.h"
#include "Game.h"

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
    virtual std::vector<std::string> getHighScores() const = 0;
    virtual void submitGameStatistics(const std::string& username, const GameData& gameData) = 0;

    // QUESTIONS operations
    virtual std::list<Question> getQuestions(int amount) const = 0;

    // CRYPTO_KEYS operations
    virtual void storeKey(const std::string& keyId, const std::string& keyValue) = 0;
    virtual std::string loadKey(const std::string& keyId) const = 0;
};