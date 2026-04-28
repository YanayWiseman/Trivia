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
    if(isUserInfoValid(user))
    {
        if (!this->m_database->doesUserExist(user.username))
        {
            return this->m_database->addNewUser(user);
        }
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

/*
    Method to check if the user's information is valid using regex expressions
    input: the userInfo struct
    output: true if valid and false otherwise
*/

bool LoginManager::isUserInfoValid(const UserInfo& user)
{
    return isPasswordValid(user.password) && isEmailValid(user.email) && isAddressValid(user.address) && isPhoneValid(user.phone) && isBirthDateValid(user.birthdate);
}

/*
    Method to check if the user's password is valid using regex expressions
    input: the user's password
    output: true if valid and false otherwise
*/

bool LoginManager::isPasswordValid(const std::string& password)
{

}

/*
    Method to check if the user's email is valid using regex expressions
    input: the user's email
    output: true if valid and false otherwise
*/

bool LoginManager::isEmailValid(const std::string& email)
{

}

/*
    Method to check if the user's address is valid using regex expressions
    input: the user's address
    output: true if valid and false otherwise
*/

bool LoginManager::isAddressValid(const std::string& address)
{

}

/*
    Method to check if the user's phone is valid using regex expressions
    input: the user's phone
    output: true if valid and false otherwise
*/

bool LoginManager::isPhoneValid(const std::string& phone)
{

}

/*
    Method to check if the user's birthdate is valid using regex expressions
    input: the user's birthdate
    output: true if valid and false otherwise
*/

bool LoginManager::isBirthDateValid(const std::string& birthDate)
{

}