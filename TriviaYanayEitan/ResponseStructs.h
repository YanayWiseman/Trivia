#pragma once

#include <string>
#include <map>
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
	std::string clientToServerKeyId;
	std::string serverToClientKeyId;
	std::string clientToServerKey;
	std::string serverToClientKey;
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
	unsigned int status;
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
	unsigned int roomId;
	unsigned int status;
};

// V3 response structs

struct CloseRoomResponse
{
	unsigned int status;
};

struct StartGameResponse
{
	unsigned int status;
};

struct GetRoomStateResponse
{
    unsigned int status = 0;
    RoomStatus hasGameBegun = Open;
    std::vector<std::string> players;
    unsigned int questionsCount = 0;
    unsigned int answerTimeout = 0;
};

struct LeaveRoomResponse
{
	unsigned int status;
};


// V4 response structs

struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

struct GetGameResultsResponse
{
	unsigned int status;
	std::vector<PlayerResults> results;
};

struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
};

struct GetQuestionResponse
{
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
	std::string imageBase64;  // empty means no image
};

struct LeaveGameResponse
{
	unsigned int status;
};

struct OneVsOneStatusResponse
{
	unsigned int status;
	unsigned int queueStatus;
	unsigned int roomId;
	unsigned int gameId;
	unsigned int questionsCount;
	unsigned int answerTimeout;
	unsigned int secondsUntilStart;
	std::string opponentUsername;
};


struct AddQuestionResponse
{
	unsigned int status;
};