#pragma once

#include "CryptoAlgorithm.h"

class CryptoManager
{
public:
    //Methods
    CryptoAlgorithm* createOTP(const std::string& username);
    //CryptoAlgorithm* createRSA(const std::string& username);
    //CryptoAlgorithm* createAES(const std::string& username);
};