#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include "RoomManager.h"
#include "ResponseStructs.h"
#include "Constants.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class RoomAdminRequestHandler : public IRequestHandler
{
private:
	//fields
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	// methods
	RequestResult getErrorResponse(const std::string& errorMsg) const;
	RequestResult closeRoom(const RequestInfo& requestInfo);
	RequestResult startGame(const RequestInfo& requestInfo); 
	RequestResult getRoomState(const RequestInfo& requestInfo);

public:
	// c'tor
	RoomAdminRequestHandler(const std::string& username, const RoomData& roomData);

	// methods
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;
};