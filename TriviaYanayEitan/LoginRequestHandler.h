#pragma once

#include <iostream>
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"


class LoginRequestHandler : public IRequestHandler
{
public:
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;
};