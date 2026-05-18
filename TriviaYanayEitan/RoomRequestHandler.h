#pragma once

#include "IRequestHandler.h"

class RoomRequestHandler : public IRequestHandler
{
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) const = 0;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) const = 0;
	RequestResult getRoomState(const RequestInfo& info) const;
};