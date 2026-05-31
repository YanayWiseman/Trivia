#pragma once

#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestStructs.h"
#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"


class RoomRequestHandler : public IRequestHandler
{
protected:
	//fields
	Room& m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	// methods
	RequestResult getRoomState(const RequestInfo& requestInfo) const;
	RequestResult getPlayersInRoom(const RequestInfo& requestInfo) const;


public:
	// c'tor
	RoomRequestHandler(const LoggedUser& user, Room& room);

	// methods
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) const = 0;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) const = 0;
};