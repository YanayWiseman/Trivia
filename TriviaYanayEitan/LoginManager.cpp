#include "LoginManager.h"


/*
    Function constructs LoginManager object.
    Input: database (IDatabase*)
    Output: None (constructor)
*/

LoginManager::LoginManager(IDatabase* database)
{
    m_database = database;
}


/*
    Function signs user up, adding him to the database.
    Input: username (const std::string&), email (const std::string&), password (const std::string&)
    Output: Whether function succeeded or not (Status)
*/

Status LoginManager::signup(const std::string& username, const std::string& email, const std::string& password)
{
    if (!this->m_database->doesUserExist(username))
    {
        return this->m_database->addNewUser(username, password, email);
    }

    return Failiure;
}


/*
    Function logs user into program, adding him to the logged users vector after confirming his details.
    Input: username (const std::string&), password (const std::string&)
    Output: Whether function succeeded or not (Status)
*/

Status LoginManager::login(const std::string& username, const std::string& password)
{
    if (this->m_database->doesUserExist(username))
    {
        if (this->m_database->doesPasswordMatch(username, password))
        {
            auto it = std::find_if(this->m_loggedUsers.begin(), this->m_loggedUsers.end(), [&](const LoggedUser& user)
                {
                    return user.getUsername() == username;
                });

            if (it == this->m_loggedUsers.end())
            {
                LoggedUser newUsr(username);
                this->m_loggedUsers.push_back(newUsr);
                return Success;
            }
        }
    }

    return Failiure;
}


/*
    Function logs user out of the program, removing him from the logged users vector.
    Input: username (const std::string&)
    Output: Whether function succeeded or not (Status)
*/

void LoginManager::logout(const std::string& username)
{
    auto it = std::find_if(this->m_loggedUsers.begin(), this->m_loggedUsers.end(), [&](const LoggedUser& user)
    {
        return user.getUsername() == username;
    });

    if (it != this->m_loggedUsers.end())
    {
        this->m_loggedUsers.erase(it);
    }
}
