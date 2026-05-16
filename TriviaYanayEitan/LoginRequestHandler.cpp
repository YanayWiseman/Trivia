#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"

/*
	Constructor for the LoginRequestHandler object
	input: the handler factory
	output: no return values
*/

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory) {}

/*
	Method to check if the request code is valid
	input: the request info
	output: true if valid and false otherwise
*/

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
	if (requestInfo.buffer.size() == 0)
	{
		return false;
	}

	if (requestInfo.id == LOGIN_REQUEST || requestInfo.id == SIGNUP_REQUEST)
	{
		return true;
	}

	return false;
}

/*
	Method to handle the request (send it and return the result)
	input: the request info
	output: the request result
*/

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo) const
{
	if (!isRequestRelevant(requestInfo))
	{
		return getErrorResponse("error: Invalid request!");
	}

	try
	{
		switch (requestInfo.id)
		{
			case LOGIN_REQUEST:
			{
				return login(requestInfo);
			}
			case SIGNUP_REQUEST:
			{
				return signup(requestInfo);
			}
			default:
			{
				return getErrorResponse("error: Wrong request format!");
			}
		}
	}
	catch (const std::exception& error)
	{
		return getErrorResponse(error.what());
	}
}


/*
	Function logs in requested user and returns server response.
	Input: info (const RequestInfo&)
	Output: Request result (RequestResult)
*/

RequestResult LoginRequestHandler::login(const RequestInfo& info) const
{
	RequestResult result;
	LoginResponse loginResponse;
	LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);

	loginResponse.status = this->m_handlerFactory.getLoginmanager().login(loginRequest.username, loginRequest.password);

	result.response = JsonResponsePacketSerializer::serializeResponse(loginResponse);

	if (loginResponse.status == Failiure)
	{
		result.newHandler = const_cast<LoginRequestHandler*>(this);
	}
	else
	{
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(loginRequest.username);
	}

	return result;
}


/*
	Function signs up requested user and returns server response.
	Input: info (const RequestInfo&)
	Output: Request result (RequestResult)
*/

RequestResult LoginRequestHandler::signup(const RequestInfo& info) const
{
	RequestResult result;
	SignupResponse signupResponse;
	SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);

	UserInfo user;
	user.username = signupRequest.username;
	user.password = signupRequest.password;
	user.email = signupRequest.email;
	user.address = signupRequest.address;
	user.phone = signupRequest.phone;
	user.birthdate = signupRequest.birthdate;

	signupResponse.status = this->m_handlerFactory.getLoginmanager().signup(user);

	result.response = JsonResponsePacketSerializer::serializeResponse(signupResponse);

	if (signupResponse.status == Failiure)
	{
		result.newHandler = const_cast<LoginRequestHandler*>(this);
	}
	else
	{
		// dont create a MenuRequestHandler yet, we need to log in first
	}

	return result;
}


/*
	Function serializes and returns error response.
	Input: errorMsg (const std::string&)
	Output: Request result (RequestResult)
*/

RequestResult LoginRequestHandler::getErrorResponse(const std::string& errorMsg) const
{
	RequestResult result;

	ErrorResponse errorResponse;
	errorResponse.error = errorMsg;

	result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	result.newHandler = nullptr;

	return result;
}
