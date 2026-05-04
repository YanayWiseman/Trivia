#pragma once

#include <string>
#include <vector>
#include "Room.h"

// V1 response structs

struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct ErrorResponse
{
	std::string error;
};

// V2 response structs

struct LogoutResponse
{
	unsigned int status;
};

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
}; 

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;
};

struct getHighScoreResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
};

struct getPersonalStatsResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
};