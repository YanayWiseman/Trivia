#include "RequestHandlerFactory.h"


/*
    Function constructs handler factory.
    Input: database (IDatabase)
    Output: None (Constructor)
*/

RequestHandlerFactory::RequestHandlerFactory(IDatabase* database) : m_loginManager(LoginManager::getInstance(database)), m_StatisticsManager(StatisticsManager::getInstance(database)), m_database(database) {}

/*
    Function creates a new request handler and returns it.
    Input: None
    Output: New LoginRequestHandler object (LoginRequestHandler*)
*/

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this);
}

/*
    Function returns login manager of the handler factory.
    Input: None
    Output: Login manager (LoginManager&)
*/

LoginManager& RequestHandlerFactory::getLoginmanager() const
{
    return this->m_loginManager;
}

/*
    Method returns statistics manager of the handler factory.
    Input: no input values
    Output: statistics manager
*/

StatisticsManager& RequestHandlerFactory::getStatisticsManager() const
{
    return this->m_StatisticsManager;
}

/*
    Function creates single instance of RequestHandlerFactory.
    Input: None
    Output: New RequestHandlerFactory instance (RequestHandlerFactory&)
*/

RequestHandlerFactory& RequestHandlerFactory::getInstance(IDatabase* database)
{
    static RequestHandlerFactory instance(database);
    return instance;
}
