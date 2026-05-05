#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "LoggedUser.h"

/*
    Function constructs handler factory.
    Input: database (IDatabase)
    Output: None (Constructor)
*/

RequestHandlerFactory::RequestHandlerFactory()
    : m_loginManager(LoginManager::getInstance()),
    m_StatisticsManager(StatisticsManager::getInstance()),
        m_roomManager(RoomManager::getInstance())
{

}

/*
    Function creates a new menu request handler and returns it.
    Input: None
    Output: New MenuRequestHandler object(LoginRequestHandler*)
*/

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& usr)
{
    return new MenuRequestHandler(*this, usr.getUsername());
}


/*
    Function creates a new login request handler and returns it.
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
    Method returns room manager of the handler factory.
    Input: no input values
    Output: room manager
*/

RoomManager& RequestHandlerFactory::getRoomManager() const
{
    return this->m_roomManager;
}


/*
    Function creates single instance of RequestHandlerFactory.
    Input: None
    Output: New RequestHandlerFactory instance (RequestHandlerFactory&)
*/

RequestHandlerFactory& RequestHandlerFactory::getInstance()
{
    static RequestHandlerFactory instance;
    return instance;
}
