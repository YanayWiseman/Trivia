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

Game& GameManager::createGame(const Room& room, const std::vector<Question>& questionsToAdd)
{
	Game game(room.getData().id, room.getData().timePerQuestion);

	std::list<Question> questionsList = this->m_database->getQuestions(AMOUNT_OF_QUESTIONS);
	std::vector<Question> questionsVector(questionsList.begin(), questionsList.end());

	// add the new questions to the questions vector
	int i = 0;
	for (i = 0; i < questionsToAdd.size(); i++)
	{
		questionsVector.push_back(questionsToAdd[i]);
	}

	game.setQuestions(questionsVector);
	
	std::vector<std::string> usernames = room.getAllUsers();
	std::vector<LoggedUser> loggedUsers;

	for (int i = 0; i < usernames.size(); i++)
	{
		loggedUsers.push_back(LoggedUser(usernames[i]));
	}

	game.setPlayers(loggedUsers);

	this->m_games.push_back(game);

	return this->m_games.back();
}


/*
	Function returns a game by its id
	Input: gameId (unsigned int)
	Output: game with the matching id (Game&)
*/

Game& GameManager::getGame(unsigned int gameId)
{
	for (int i = 0; i < this->m_games.size(); i++)
	{
		if (this->m_games[i].getGameId() == gameId)
		{
			return this->m_games[i];
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
	for (auto it = this->m_games.begin(); it != this->m_games.end(); it++)
	{
		if (it->getGameId() == gameId)
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
	this->m_database->submitGameStatistics(username, gameData);
}