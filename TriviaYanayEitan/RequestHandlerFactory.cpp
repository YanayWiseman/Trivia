#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "LoggedUser.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

/*
    Function constructs handler factory.
    Input: database (IDatabase)
    Output: None (Constructor)
*/

RequestHandlerFactory::RequestHandlerFactory()
    : m_loginManager(LoginManager::getInstance()),
    m_StatisticsManager(StatisticsManager::getInstance()),
    m_roomManager(RoomManager::getInstance()), m_gameManager(GameManager::getInstance())
{

}

/*
    Function creates a new menu request handler and returns it.
    Input: None
    Output: New MenuRequestHandler object(LoginRequestHandler*)
*/

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& usr)
{
    return new MenuRequestHandler(usr);
}

/*
    Function creates a new RoomMemberRequestHandler and returns it.
    Input: None
    Output: New RoomMemberRequestHandler object(RoomMemberRequestHandler*)
*/

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& usr, Room& room)
{
    return new RoomMemberRequestHandler(usr, room);
}

/*
    Function creates a new RoomAdminRequestHandler and returns it.
    Input: None
    Output: New RoomAdminRequestHandler object(RoomAdminRequestHandler*)
*/

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& usr, Room& room)
{
    return new RoomAdminRequestHandler(usr, room);
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

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(Game& game, const LoggedUser& usr)
{
    return new GameRequestHandler(game, usr);
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
    Method returns game manager of the handler factory.
    Input: no input values
    Output: game manager
*/

GameManager& RequestHandlerFactory::getGameManager() const
{
    return this->m_gameManager;
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
