#pragma once

#include "LoginManager.h"
#include "IDatabase.h"
#include "LoginRequestHandler.h"

class RequestHandlerFactory
{
private:
	LoginManager& m_loginManager;
	IDatabase* m_database; 
	RequestHandlerFactory(IDatabase* database);

public:
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginmanager();

	static RequestHandlerFactory& getInstance(IDatabase* database);

	RequestHandlerFactory(const RequestHandlerFactory& other) = delete;
	RequestHandlerFactory& operator=(const RequestHandlerFactory& other) = delete;
};