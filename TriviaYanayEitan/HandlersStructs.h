#pragma once

#include <ctime>

#include "typedefs.h"
class IRequestHandler;

typedef struct RequestResult
{
	Buffer response;
	IRequestHandler* newHandler = nullptr;
} RequestResult;

typedef struct RequestInfo
{
	Byte id = 0;
	time_t receivalTime = 0;
	Buffer buffer;
} RequestInfo;