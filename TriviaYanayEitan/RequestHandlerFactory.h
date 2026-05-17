#pragma once


#include "LoginManager.h"
#include "StatisticsManager.h"
#include "IDatabase.h"
#include "RoomManager.h"

class LoginRequestHandler;
class MenuRequestHandler;
class RoomMemberRequestHandler;
class LoggedUser;


class RequestHandlerFactory
{
private:
	LoginManager& m_loginManager;
	StatisticsManager& m_StatisticsManager;
	RoomManager& m_roomManager;
	IDatabase* m_database; 
	RequestHandlerFactory();

public:

	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& usr);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& usr, const Room& room);
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginmanager() const;
	StatisticsManager& getStatisticsManager() const;
	RoomManager& getRoomManager() const;

	static RequestHandlerFactory& getInstance();

	RequestHandlerFactory(const RequestHandlerFactory& other) = delete;
	RequestHandlerFactory& operator=(const RequestHandlerFactory& other) = delete;
};