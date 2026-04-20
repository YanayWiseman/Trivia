#pragma once

#include "HandlersStructs.h"
#include "Constants.h"

class IRequestHandler
{
public:
	// methods
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) const = 0;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) const = 0;
};