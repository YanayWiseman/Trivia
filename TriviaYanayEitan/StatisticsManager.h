#pragma once

#include <string>
#include <vector>
#include "IDatabase.h"

class StatisticsManager
{
private:
	// fields
	IDatabase* m_database;

public:
	// c'tor and d'tor
	StatisticsManager(IDatabase* database);
	~StatisticsManager();


};
