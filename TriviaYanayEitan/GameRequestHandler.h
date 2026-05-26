#pragma once

#include "IRequestHandler.h"
#include "Game.h"
#include "LoggedUser.h"
#include "GameManager.h"
#include "HandlersStructs.h"
#include "ResponseStructs.h"

#define NO_ANSWER 0

class RequestHandlerFactory;



class GameRequestHandler : public IRequestHandler
{
private:
	Game& m_game;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult submitAnswer(const RequestInfo& requestInfo) const;
	RequestResult getGameResults(const RequestInfo& requestInfo) const;
	RequestResult leaveGame(const RequestInfo& requestInfo) const;
	RequestResult getQuestion(const RequestInfo& requestInfo) const;
	RequestResult getErrorResponse(const std::string& errorMsg) const;

public:
	GameRequestHandler(Game& game, const LoggedUser& user, GameManager& gameManager);

	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;

};