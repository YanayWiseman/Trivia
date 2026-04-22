#include "LoginRequestHandler.h"

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
	RequestResult result;
	result.newHandler = const_cast<LoginRequestHandler*>(this);

	if (!isRequestRelevant(requestInfo))
	{
		ErrorResponse errorResponse;
		errorResponse.error = "Invalid request";
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		return result;
	}

	try
	{
		switch (requestInfo.id)
		{
			case LOGIN_REQUEST:
			{
				LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);

				std::cout << "Calling Login..." << std::endl;

				LoginResponse loginResponse;
				loginResponse.status = 1; // temp
				result.response = JsonResponsePacketSerializer::serializeResponse(loginResponse);
				return result;
			}
			case SIGNUP_REQUEST:
			{
				SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);

				std::cout << "Calling Signup..." << std::endl;

				SignupResponse signupResponse;
				signupResponse.status = 1; // temp
				result.response = JsonResponsePacketSerializer::serializeResponse(signupResponse);
				return result;
			}
			default:
			{
				ErrorResponse errorResponse;
				errorResponse.error = "error: Wrong request format!";
				result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
				result.newHandler = nullptr;
				return result;
			}
		}
	}
	catch (const std::exception& error)
	{
		ErrorResponse errorResponse;
		errorResponse.error = error.what();
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;
		return result;
	}
}