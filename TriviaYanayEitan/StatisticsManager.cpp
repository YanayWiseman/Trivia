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

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username) const
{
    std::vector<std::string> res;

    res.push_back(username);
    res.push_back(std::to_string(this->m_database->getPlayerAverageAnswerTime(username)));
    res.push_back(std::to_string(this->m_database->getNumOfCorrectAnswers(username)));
    res.push_back(std::to_string(this->m_database->getNumOfPlayerGames(username)));
    res.push_back(std::to_string(this->m_database->getNumOfTotalAnswers(username)));

    return res;
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
