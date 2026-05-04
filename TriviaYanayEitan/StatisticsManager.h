#pragma once

#include <string>
#include <vector>
#include "IDatabase.h"
#include "UserInfo.h"

class StatisticsManager
{
private:
	// fields
	IDatabase* m_database;

public:
	// c'tor and d'tor
	StatisticsManager(IDatabase* database);
	~StatisticsManager() = default;

	// methods
	std::vector<std::pair<std::string, int>> getHighScores() const;
	UserStatistics getUserStatistics(const std::string& username) const;
};
