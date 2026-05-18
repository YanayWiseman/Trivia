#pragma once
#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class RoomMemberRequestHandler : public IRequestHandler
{
private:
	//fields
	Room& m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	// methods
	RequestResult leaveRoom(const RequestInfo& info) const;
	RequestResult getRoomState(const RequestInfo& info) const;
	RequestResult getErrorResponse(const std::string& errorMsg) const;

public:
	// c'tor
	RoomMemberRequestHandler(const LoggedUser& user, Room& room);

	// methods
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;


};