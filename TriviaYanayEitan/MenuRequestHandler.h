#pragma once

#include <random>
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
#include "HandlersStructs.h"
#include "Constants.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
private:
	//fields
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;

	// methods
	RequestResult signout(const RequestInfo& requestInfo) const;
	RequestResult getRooms(const RequestInfo& requestInfo) const;
	RequestResult getPlayersInRoom(const RequestInfo& requestInfo) const;
	RequestResult getPersonalStats(const RequestInfo& requestInfo) const;
	RequestResult getHighScore(const RequestInfo& requestInfo) const;
	RequestResult joinRoom(const RequestInfo& requestInfo) const;
	RequestResult createRoom(const RequestInfo& requestInfo) const;
	RequestResult getErrorResponse(const std::string& errorMsg) const;

public:
	// c'tor
	MenuRequestHandler(const LoggedUser& user);

	// methods
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;
};