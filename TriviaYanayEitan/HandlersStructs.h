#pragma once

#include <ctime>

#include "typedefs.h"
class IRequestHandler;

typedef struct RequestResult
{
	Buffer response;
	IRequestHandler* newHandler;
} RequestResult;

typedef struct RequestInfo
{
	Byte id;
	time_t recievalTime;
	Buffer buffer;
} RequestInfo;