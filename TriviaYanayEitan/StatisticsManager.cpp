#include "StatisticsManager.h"

/*
    Method constructs StatisticsManager object
    Input: the database
    Output: no return values
*/

StatisticsManager::StatisticsManager()
{
    this->m_database = &SqliteDatabase::getInstance();
}

/*
    Method to get the top 5 best scores
    input: no input values
    output: vector of top 5 best scores
*/

std::vector<std::pair<std::string, int>> StatisticsManager::getHighScores() const
{
    return this->m_database->getHighScores();
}

/*
    Method to get a user's statistics by the username
    input: the username
    output: vector of its statistics
*/

UserStatistics StatisticsManager::getUserStatistics(const std::string& username) const
{
    UserStatistics userStats;

    userStats.username = username;
    userStats.averageTime = this->m_database->getPlayerAverageAnswerTime(username);
    userStats.correctAnswers = this->m_database->getNumOfCorrectAnswers(username);
    userStats.gamesCount = this->m_database->getNumOfPlayerGames(username);
    userStats.numAnswered = this->m_database->getNumOfTotalAnswers(username);

    return userStats;
}

/*
    Method creates single instance of LoginManager.
    Input: no input values
    Output: New LoginManager instance (LoginManager&)
*/

StatisticsManager& StatisticsManager::getInstance()
{
    static StatisticsManager instance;
    return instance;
}
