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
