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

	// methods
	StatisticsManager(IDatabase* database);
public:
	// d'tor
	~StatisticsManager() = default;

	// methods
	std::vector<std::pair<std::string, int>> getHighScores() const;
	UserStatistics getUserStatistics(const std::string& username) const;

	static StatisticsManager& getInstance(IDatabase* database);

	StatisticsManager(const StatisticsManager& other) = delete;
	StatisticsManager& operator=(const StatisticsManager& other) = delete;
};
