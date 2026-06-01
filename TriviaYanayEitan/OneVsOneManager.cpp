#include "OneVsOneManager.h"


/*
	Constructor for OneVsOneManager object.
	Input: None
	Output: None (Constructor)
*/

OneVsOneManager::OneVsOneManager() :
	m_waitingUser(""),
	m_hasWaitingUser(false)
{
}


/*
	Function returns single instance of OneVsOneManager.
	Input: None
	Output: OneVsOneManager instance (OneVsOneManager&)
*/

OneVsOneManager& OneVsOneManager::getInstance()
{
	static OneVsOneManager instance;
	return instance;
}


/*
	Function joins user to 1v1 queue or creates a match if another user is waiting.
	Input: user (const LoggedUser&), matchData (OneVsOneMatchData&)
	Output: Queue status (OneVsOneQueueStatus)
*/

OneVsOneQueueStatus OneVsOneManager::joinQueue(const LoggedUser& user, OneVsOneMatchData& matchData)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);

	// check if this user already has a created match waiting for him
	std::map<std::string, OneVsOneMatchData>::iterator existingMatch = this->m_matches.find(user.getUsername());

	if (existingMatch != this->m_matches.end())
	{
		matchData = existingMatch->second;
		return OneVsOneMatched;
	}

	// if this exact user is already waiting, do not add him twice
	if (this->m_hasWaitingUser && this->m_waitingUser.getUsername() == user.getUsername())
	{
		return OneVsOneWaiting;
	}

	// if another user is waiting, create a 1v1 match
	if (this->m_hasWaitingUser)
	{
		RoomData roomData;

		roomData.name = "1v1";
		roomData.maxPlayers = 2;
		roomData.numOfQuestionsInGame = ONE_VS_ONE_QUESTIONS_COUNT;
		roomData.timePerQuestion = ONE_VS_ONE_TIME_PER_QUESTION;
		roomData.status = InGame;

		unsigned int roomId = RoomManager::getInstance().createRoomWithRandomId(this->m_waitingUser, roomData);

		Room& room = RoomManager::getInstance().getRoom(roomId);
		room.addUser(user);

		Game& game = GameManager::getInstance().createGame(room, std::vector<Question>());

		// both clients should start after the same countdown
		std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now() + std::chrono::seconds(ONE_VS_ONE_COUNTDOWN_SECONDS);

		OneVsOneMatchData firstPlayerMatchData;
		firstPlayerMatchData.roomId = roomId;
		firstPlayerMatchData.gameId = game.getGameId();
		firstPlayerMatchData.opponentUsername = user.getUsername();
		firstPlayerMatchData.startTime = startTime;

		OneVsOneMatchData secondPlayerMatchData;
		secondPlayerMatchData.roomId = roomId;
		secondPlayerMatchData.gameId = game.getGameId();
		secondPlayerMatchData.opponentUsername = this->m_waitingUser.getUsername();
		secondPlayerMatchData.startTime = startTime;

		this->m_matches[this->m_waitingUser.getUsername()] = firstPlayerMatchData;
		this->m_matches[user.getUsername()] = secondPlayerMatchData;

		matchData = secondPlayerMatchData;

		// clear waiting slot
		this->m_hasWaitingUser = false;

		return OneVsOneMatched;
	}

	// nobody is waiting, so this user becomes the waiting player
	this->m_waitingUser = user;
	this->m_hasWaitingUser = true;

	return OneVsOneWaiting;
}


/*
	Function returns users current 1v1 queue status.
	Input: user (const LoggedUser&), matchData (OneVsOneMatchData&)
	Output: Queue status (OneVsOneQueueStatus)
*/

OneVsOneQueueStatus OneVsOneManager::getStatus(const LoggedUser& user, OneVsOneMatchData& matchData)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);

	// if the user was already matched, return the match data
	std::map<std::string, OneVsOneMatchData>::iterator matchIt = this->m_matches.find(user.getUsername());

	if (matchIt != this->m_matches.end())
	{
		matchData = matchIt->second;
		return OneVsOneMatched;
	}

	// if the user is still the waiting player, return waiting
	if (this->m_hasWaitingUser && this->m_waitingUser.getUsername() == user.getUsername())
	{
		return OneVsOneWaiting;
	}

	return OneVsOneNotQueued;
}


/*
	Function removes user from the 1v1 queue.
	Input: user (const LoggedUser&)
	Output: None
*/

void OneVsOneManager::cancelQueue(const LoggedUser& user)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);

	if (this->m_hasWaitingUser && this->m_waitingUser.getUsername() == user.getUsername())
	{
		this->m_hasWaitingUser = false;
	}

	this->m_matches.erase(user.getUsername());
}


/*
	Function removes user's stored match data.
	Input: username (const std::string&)
	Output: None
*/

void OneVsOneManager::removeMatch(const std::string& username)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);

	this->m_matches.erase(username);
}