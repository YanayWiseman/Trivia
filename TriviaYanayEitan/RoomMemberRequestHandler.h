#pragma once

#include "RoomRequestHandler.h"
#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class RoomMemberRequestHandler : public RoomRequestHandler
{
private:

	// methods
	RequestResult leaveRoom(const RequestInfo& info) const;
	RequestResult getErrorResponse(const std::string& errorMsg) const;

public:
	RoomMemberRequestHandler(const LoggedUser& user, Room& room);

	// methods
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;


};