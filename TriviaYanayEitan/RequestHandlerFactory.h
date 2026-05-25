#pragma once


#include "GameRequestHandler.h"
#include "LoginManager.h"
#include "StatisticsManager.h"
#include "IDatabase.h"
#include "RoomManager.h"
#include "GameManager.h"

class LoginRequestHandler;
class MenuRequestHandler;
class LoggedUser;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;

class RequestHandlerFactory
{
private:
	LoginManager& m_loginManager;
	StatisticsManager& m_StatisticsManager;
	RoomManager& m_roomManager;
	GameManager& m_gameManager;

	IDatabase* m_database; 
	RequestHandlerFactory();

public:

	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& usr);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& usr, Room& room);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& usr,  Room& room);
	LoginRequestHandler* createLoginRequestHandler();
	GameRequestHandler* createGameRequestHandler();
	LoginManager& getLoginmanager() const;
	StatisticsManager& getStatisticsManager() const;
	RoomManager& getRoomManager() const;
	GameManager& getGameManager() const;

	static RequestHandlerFactory& getInstance();

	RequestHandlerFactory(const RequestHandlerFactory& other) = delete;
	RequestHandlerFactory& operator=(const RequestHandlerFactory& other) = delete;
};