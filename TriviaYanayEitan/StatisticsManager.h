#pragma once

#include <string>
#include <vector>
#include "IDatabase.h"
#include "UserInfo.h"
#include "SqliteDatabase.h"

class StatisticsManager
{
private:
	// fields
	IDatabase* m_database;

	// methods
	StatisticsManager();
public:
	// d'tor
	~StatisticsManager() = default;

	// methods
	std::vector<std::pair<std::string, int>> getHighScores() const;
	std::vector<std::string> getUserStatistics(const std::string& username) const;

	static StatisticsManager& getInstance();

	StatisticsManager(const StatisticsManager& other) = delete;
	StatisticsManager& operator=(const StatisticsManager& other) = delete;
};
