#pragma once

#include "GameManager.h"
#include "GameRequestHandler.h"
#include "RoomRequestHandler.h"
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include "RoomManager.h"
#include "ResponseStructs.h"
#include "Constants.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "MenuRequestHandler.h"

class RoomAdminRequestHandler : public RoomRequestHandler
{
private:
	
	// methods
	RequestResult closeRoom(const RequestInfo& requestInfo) const;
	RequestResult startGame(const RequestInfo& requestInfo) const;
	RequestResult getErrorResponse(const std::string& errorMsg) const;

public:
	RoomAdminRequestHandler(const LoggedUser& loggedUser, Room& room);

	// methods
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;
};