#include "LoginManager.h"

Status LoginManager::signup(const std::string& username, const std::string& email, const std::string& password)
{
    return this->m_database->addNewUser(username, password, email);
}

Status LoginManager::login(const std::string& username, const std::string& password)
{
    if (this->m_database->doesUserExist(username))
    {
        if (this->m_database->doesPasswordMatch(username, password))
        {
            LoggedUser newUsr(username);
            this->m_loggedUsers.push_back(newUsr);
            return Success;
        }
    }

    return Failiure;
}

void LoginManager::logout(const std::string& username)
{
    auto it = std::find_if(m_loggedUsers.begin(), m_loggedUsers.end(), [&](const LoggedUser& user)
    {
        return user.getUsername() == username;
    });

    if (it != m_loggedUsers.end())
    {
        m_loggedUsers.erase(it);
    }
}
