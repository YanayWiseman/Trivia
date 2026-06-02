#pragma once

#include <winsock2.h>
#include <unordered_map>
#include "CryptoAlgorithm.h"

struct SessionData
{
	std::string username;
	CryptoAlgorithm* crypto;
};

class SessionManager
{
private:
	// Fields
	std::unordered_map<SOCKET, SessionData> m_sessions;

public:
	// Methods
	void createSession(SOCKET sock, const std::string& username);
	SessionData& getSession(SOCKET sock);
	void removeSession(SOCKET sock);
};