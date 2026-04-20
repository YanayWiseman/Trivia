#pragma once

#include "HandlersStructs.h"

class IRequestHandler
{
public:
	// methods
	virtual bool isRequestRelevant(RequestInfo requestInfo) const = 0;
	virtual RequestResult handleRequest(RequestInfo requestInfo) const = 0;
};