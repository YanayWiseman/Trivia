#include "GameManager.h"

/*
	Constructor for the GameManager Object
	input: no input values
	output: no return values
*/

GameManager::GameManager()
{
	this->m_database = &SqliteDatabase::getInstance();
}

/*
	Function creates single instance of GameManager.
	Input: None
	Output: New GameManager instance (GameManager&)
*/

GameManager& GameManager::getInstance()
{
	static GameManager instance;
	return instance;
}

/*
	Method to create a room in the game manager's map
	Input: the room of the game
	Output: the new Game struct created
*/

Game& GameManager::createGame(const Room& room, int questionCount, const std::vector<Question>& questionsToAdd)
{
	auto game = std::make_unique<Game>(room.getData().id, room.getData().timePerQuestion);

	std::list<Question> questionsList = m_database->getQuestions(questionCount);
	std::vector<Question> questionsVector(questionsList.begin(), questionsList.end());

	for (const Question& question : questionsToAdd)
	{
		questionsVector.push_back(question);
	}

	game->setQuestions(questionsVector);

	std::vector<std::string> usernames = room.getAllUsers();
	std::vector<LoggedUser> loggedUsers;

	for (const std::string& username : usernames)
	{
		loggedUsers.emplace_back(username);
	}

	game->setPlayers(loggedUsers);

	std::lock_guard<std::mutex> lock(m_mutex);

	m_games.push_back(std::move(game));
	return *m_games.back();
}


/*
	Function returns a game by its id
	Input: gameId (unsigned int)
	Output: game with the matching id (Game&)
*/

Game& GameManager::getGame(unsigned int gameId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	for (const auto& game : m_games)
	{
		if (game->getGameId() == gameId)
		{
			return *game;
		}
	}

	throw std::runtime_error("error: Game not found");
}

/*
	Method to delete a game from the game manager's map
	Input: the game's id
	Output: no return values
*/

void GameManager::deleteGame(unsigned int gameId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	for (auto it = this->m_games.begin(); it != this->m_games.end(); it++)
	{
		if ((*it)->getGameId() == gameId)
		{
			// delete the game
			this->m_games.erase(it);
			break;
		}
	}

}

/*
	Private Method to submit the game status to the database
	input: the game's data
	output: no return values
*/

void GameManager::submitGameStatusToDB(const std::string& username, const GameData& gameData)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	this->m_database->submitGameStatistics(username, gameData);
}