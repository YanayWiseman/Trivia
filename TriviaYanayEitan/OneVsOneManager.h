#pragma once
#include <map>
#include <mutex>
#include <string>
#include <chrono>
#include "LoggedUser.h"
#include "RoomManager.h"
#include "GameManager.h"

#define ONE_VS_ONE_QUESTIONS_COUNT 10
#define ONE_VS_ONE_TIME_PER_QUESTION 10
#define ONE_VS_ONE_COUNTDOWN_SECONDS 5

enum OneVsOneQueueStatus
{
	OneVsOneNotQueued = 0,
	OneVsOneWaiting = 1,
	OneVsOneMatched = 2
};

struct OneVsOneMatchData
{
	unsigned int roomId;
	unsigned int gameId;
	std::string opponentUsername;
	std::chrono::steady_clock::time_point startTime;
};

class OneVsOneManager
{
private:
	LoggedUser m_waitingUser;
	bool m_hasWaitingUser;

	std::map<std::string, OneVsOneMatchData> m_matches;
	std::mutex m_mutex;

	OneVsOneManager();

public:
	OneVsOneQueueStatus joinQueue(const LoggedUser& user, OneVsOneMatchData& matchData);
	OneVsOneQueueStatus getStatus(const LoggedUser& user, OneVsOneMatchData& matchData);
	void cancelQueue(const LoggedUser& user);
	void removeMatch(const std::string& username);

	static OneVsOneManager& getInstance();

	OneVsOneManager(const OneVsOneManager& other) = delete;
	OneVsOneManager& operator=(const OneVsOneManager& other) = delete;
};