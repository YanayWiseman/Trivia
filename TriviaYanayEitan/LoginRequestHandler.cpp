#include "LoginRequestHandler.h"

bool LoginRequestHandler::isRequestRelevant(RequestInfo requestInfo) const
{
	return false;   // REMOVE
}
RequestResult LoginRequestHandler::handleRequest(RequestInfo requestInfo) const
{
	return RequestResult();  // REMOVE
}