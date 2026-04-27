#pragma once

#include <iostream>
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "ResponseStructs.h"


class RequestHandlerFactory;


class LoginRequestHandler : public IRequestHandler
{
private:
	RequestHandlerFactory& m_handlerFactory;
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory);
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) const override;
	RequestResult login(const RequestInfo& info) const;
	RequestResult signup(const RequestInfo& info) const;
	RequestResult getErrorResponse(const std::string& errorMsg) const;
};