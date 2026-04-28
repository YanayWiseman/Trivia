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

Status LoginManager::signup(const UserInfo& user)
{
    if (!isValidEmail(user.email) || !isValidPassword(user.password) || !isValidPhone(user.phone)) return Failiure;
    if (!this->m_database->doesUserExist(user.username))
    {
        return this->m_database->addNewUser(user);
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


// Helpers

bool validateUserInfo(const UserInfo& user)
{

}
bool isPasswordValid(const std::string& password)
{

}
bool isEmailValid(const std::string& email)
{

}
bool isAddressValid(const std::string& address)
{

}
bool isPhoneValid(const std::string& phone)
{

}
bool isBirthDateValid(const std::string& birthDate)
{

}