#pragma once

#include "LoginManager.h"
#include "StatisticsManager.h"
#include "IDatabase.h"
#include "LoginRequestHandler.h"

class RequestHandlerFactory
{
private:
	LoginManager& m_loginManager;
	StatisticsManager& m_StatisticsManager;
	IDatabase* m_database; 
	RequestHandlerFactory();

public:
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginmanager() const;
	StatisticsManager& getStatisticsManager() const;

	static RequestHandlerFactory& getInstance();

	RequestHandlerFactory(const RequestHandlerFactory& other) = delete;
	RequestHandlerFactory& operator=(const RequestHandlerFactory& other) = delete;
};