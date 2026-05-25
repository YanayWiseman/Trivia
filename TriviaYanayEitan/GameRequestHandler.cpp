#include "GameRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "Constants.h"

/*
	Function creates GameRequestHandler object.
	Input: game (Game&), user (const LoggedUser&), gameManager (GameManager&), handlerFactory (RequestHandlerFactory&)
	Output: None (constructor)
*/

GameRequestHandler::GameRequestHandler(Game& game, const LoggedUser& user, GameManager& gameManager)
	: m_game(game), m_user(user), m_gameManager(gameManager), m_handlerFactory(RequestHandlerFactory::getInstance())
{
}


/*
	Function submits answer of current user to current game.
	Input: requestInfo (const RequestInfo&)
	Output: Request result (RequestResult)
*/

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& requestInfo) const
{
	RequestResult result;

	SubmitAnswerRequest submitAnswerRequest = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfo.buffer);

	m_game.submitAnswer(m_user, submitAnswerRequest.answerId, 0);

	SubmitAnswerResponse submitAnswerResponse;
	submitAnswerResponse.status = Success;

	result.response = JsonResponsePacketSerializer::serializeResponse(submitAnswerResponse);
	result.newHandler = const_cast<GameRequestHandler*>(this);

	return result;
}


/*
	Function returns current question for current user.
	Input: requestInfo (const RequestInfo&)
	Output: Request result (RequestResult)
*/

RequestResult GameRequestHandler::getQuestion(const RequestInfo& requestInfo) const
{
	RequestResult result;
	GetQuestionResponse getQuestionResponse;

	Question question = m_game.getQuestionsForUser(m_user);
	std::vector<std::string> answers = question.getPossibleAnswers();

	getQuestionResponse.status = Success;
	getQuestionResponse.question = question.getQuestion();

	for (unsigned int i = 0; i < answers.size(); i++)
	{
		getQuestionResponse.answers[i] = answers[i];
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(getQuestionResponse);
	result.newHandler = const_cast<GameRequestHandler*>(this);

	return result;

}


/*
	Function returns game results of current game.
	Input: requestInfo (const RequestInfo&)
	Output: Request result (RequestResult)
*/

RequestResult GameRequestHandler::getGameResults(const RequestInfo& requestInfo) const
{
	RequestResult result;
	GetGameResultsResponse getGameResultsResponse;
	std::map<std::string, GameData> players = m_game.getPlayers();

	getGameResultsResponse.status = Success;

	for (std::map<std::string, GameData>::iterator it = players.begin(); it != players.end(); ++it)
	{
		PlayerResults playerResults;

		playerResults.username = it->first;
		playerResults.correctAnswerCount = it->second.correctAnswerCount;
		playerResults.wrongAnswerCount = it->second.wrongAnswerCount;
		playerResults.avrageAnswerTime = it->second.averageAnswerTime;

		getGameResultsResponse.results.push_back(playerResults);
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(getGameResultsResponse);
	result.newHandler = const_cast<GameRequestHandler*>(this);

	return result;
}


/*
	Function removes current user from current game.
	Input: requestInfo (const RequestInfo&)
	Output: Request result (RequestResult)
*/

RequestResult GameRequestHandler::leaveGame(const RequestInfo& requestInfo) const
{
	RequestResult result;
	LeaveGameResponse leaveGameResponse;

	m_game.removePlayer(m_user);

	leaveGameResponse.status = Success;

	result.response = JsonResponsePacketSerializer::serializeResponse(leaveGameResponse);
	result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);

	return result;
}


/*
	Method to check if the request code is valid
	input: the request info
	output: true if valid and false otherwise
*/

bool GameRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
	if (requestInfo.buffer.size() == 0)
	{
		return false;
	}

	if (requestInfo.id == SUBMIT_ANSWER_REQUEST || requestInfo.id == GET_GAME_RESULT_REQUEST || requestInfo.id == LEAVE_GAME_REQUEST || requestInfo.id == GET_QUESTION_REQUEST)
	{
		return true;
	}

	return false;
}


/*
	Method to handle the request (send it and return the result)
	input: the request info
	output: the request result
*/

RequestResult GameRequestHandler::handleRequest(const RequestInfo& requestInfo) const
{
	// first check if the request is valid
	if (!isRequestRelevant(requestInfo))
	{
		return getErrorResponse("error: Invalid request!");
	}


	// then sort by request id to handle each request
	try
	{
		switch (requestInfo.id)
		{
		case SUBMIT_ANSWER_REQUEST:
			return submitAnswer(requestInfo);

		case GET_GAME_RESULT_REQUEST:
			return getGameResults(requestInfo);

		case LEAVE_GAME_REQUEST:
			return leaveGame(requestInfo);

		case GET_QUESTION_REQUEST:
			return getQuestion(requestInfo);

		default:
			return getErrorResponse("error: Wrong request format!");
		}
	}
	catch (const std::exception& error)
	{
		return getErrorResponse(error.what());
	}
}


/*
	method serializes and returns error response.
	input: the errorMsg
	output: Request result
*/

RequestResult GameRequestHandler::getErrorResponse(const std::string& errorMsg) const
{
	RequestResult result;

	ErrorResponse errorResponse;
	errorResponse.error = errorMsg;

	result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	result.newHandler = const_cast<GameRequestHandler*>(this);

	return result;
}
