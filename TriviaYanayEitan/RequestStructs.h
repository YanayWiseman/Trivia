#pragma once

#include <string>

//V1

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
    std::string username;
    std::string password;
    std::string email;
    std::string address;
    std::string phone;
    std::string birthdate;
};


// V2

struct GetPlayersInRoomRequest
{
    unsigned int roomId;
};

struct JoinRoomRequest
{
    unsigned int roomId;
};

struct CreateRoomRequest
{
    std::string roomName;
    unsigned int maxUsers;
    unsigned int questionsCount;
    unsigned int answerTimeout;
};


// V4

struct SubmitAnswerRequest
{
    unsigned int answerId;
};


// V4.0.1

struct AddQuestionRequest
{
    int correctAnswerId;
    std::string question;
    std::vector<std::string> answers;
};
