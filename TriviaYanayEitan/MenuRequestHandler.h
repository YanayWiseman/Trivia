#pragma once

#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
#include "HandlersStructs.h"
#include "Constants.h"

class MenuRequestHandler : public IRequestHandler
{
private:
	//fields
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;

	// methods
	RequestResult signout(const RequestInfo& requestInfo);
	RequestResult getRooms(const RequestInfo& requestInfo);
	RequestResult getPlayersInRoom(const RequestInfo& requestInfo);
	RequestResult getPersonalStats(const RequestInfo& requestInfo);
	RequestResult getHighScore(const RequestInfo& requestInfo);
	RequestResult joinRoom(const RequestInfo& requestInfo);
	RequestResult createRoom(const RequestInfo& requestInfo);
	RequestResult getErrorResponse(const std::string& errorMsg) const;

public:
	// c'tor
	MenuRequestHandler(RequestHandlerFactory& handlerFactory, const std::string& username);

	// methods
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;
};