#pragma once

#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	bool isRequestRelevant(RequestInfo requestInfo) const override;
	RequestResult handleRequest(RequestInfo requestInfo) const override;
};