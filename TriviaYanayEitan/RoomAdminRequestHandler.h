#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "Room.h"

class RoomAdminRequestHandler : public IRequestHandler
{
private:
	//fields
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

public:
	// c'tor
	RoomAdminRequestHandler(RequestHandlerFactory& handlerFactory, const std::string& username);

	// methods
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;
};