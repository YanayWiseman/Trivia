#include "StatisticsManager.h"

/*
    Method constructs StatisticsManager object
    Input: the database
    Output: no return values
*/

StatisticsManager::StatisticsManager(IDatabase* database)
{
    this->m_database = database;
}

/*
    Method to get the top 5 best scores
    input: no input values
    output: vector of top 5 best scores
*/

std::vector<std::pair<std::string, int>> StatisticsManager::getHighScores() const
{
    
}



std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username) const
{

}