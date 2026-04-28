#pragma once

#include "LoginManager.h"
#include "IDatabase.h"
#include "LoginRequestHandler.h"

class RequestHandlerFactory
{
private:
	LoginManager m_loginManager;
	IDatabase* m_database; 

public:
	RequestHandlerFactory(IDatabase* database);
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginmanager();
};