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
    Method to check if the user's email is valid using regex expressions
    input: the user's email
    output: true if valid and false otherwise
*/

bool LoginManager::isEmailValid(const std::string& email)
{
    // the email should be in this format:
    // blabla@something.domain

    // R before the strings means the complier will ignore things like 
    // \n \t and special characters (raw string literal), also makes it easier to read regex

    // regex explanation:
    /*
    ^ means start from the beggining of the string and read all of it
    the part before the @ can only be letters and numbers - [A-Za-z0-9]+
    then we need to see @, and then the 'something' from the pattern before the .domain
    so we put [A-Za-z0-9]+ again.
    in regex a dot . means any character (meaning its a special character like % in C) 
    so theres a special way to check of it, with a \.
    we need a .domain, the domain can be a letter or a number again
    the best part is the +$ after the brackets, it means that iit can repeat itself, like in .co.il
    */
    static const std::regex pattern(R"(^[A-Za-z0-9]+@[A-Za-z0-9]+(\.[A-Za-z0-9]+)+$)");
    return std::regex_match(email, pattern);
}

/*
    Method to check if the user's password is valid using regex expressions
    input: the user's password
    output: true if valid and false otherwise
*/

bool LoginManager::isPasswordValid(const std::string& password)
{
    // 8 or more chars, big letters, small letters, digits and special character
    /*
    ^ is the start of the string and $ is the end, together they ensure the entire password must fit this pattern from start to finish
    each set of parentheses starting with ?=.* is a "checkpoint"
    a-z checks for lowercase
    A-Z checks for uppercase
    \d checks for digits
    and [\*\&\^\%\$\#\@\!] checks for one of those chars
    length is tested by this {8,}, the comma means it can be longer then 8
    */
    static const std::regex pattern(R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[\*\&\^\%\$\#\@\!]).{8,}$)");
    return std::regex_match(password, pattern);
}

/*
    Method to check if the user's address is valid using regex expressions
    input: the user's address
    output: true if valid and false otherwise
*/

bool LoginManager::isAddressValid(const std::string& address)
{
    // this is a simple pattern: Street, Apt, City
    // I have explained mostly what this expression is doing in previous expressions as this is
    // using the same logic (Yanay)
    // the + means one or more of the expression that is in the brackets
    static const std::regex pattern(R"(^[A-Za-z ]+, \d+, [A-Za-z ]+$)");
    return std::regex_match(address, pattern);
}

/*
    Method to check if the user's phone is valid using regex expressions
    input: the user's phone
    output: true if valid and false otherwise
*/

bool LoginManager::isPhoneValid(const std::string& phone)
{
    // prefix - number
    // (0\d{1,2}) - the first char must be a 0, the next part means that there can be any digit 0-9
    // either once or twice so 07 will be fine and alsoo 053
    // dash - for the prefix and number seperator and seven digits 0-9
    static const std::regex pattern(R"(^(0\d{1,2})-\d{7}$)");
    return std::regex_match(phone, pattern);
}

/*
    Method to check if the user's birthdate is valid using regex expressions
    input: the user's birthdate
    output: true if valid and false otherwise
*/

bool LoginManager::isBirthDateValid(const std::string& birthDate)
{
    // format is DD.MM.YYYY or DD/MM/YYYY
    // (0[1-9]|[12][0-9]|3[01]) this ensures that the day is between 0 and 31, | is an OR
    //0[1-9] is 0 through 9, [12][0-9] is 10 through 29  and 3[01] is 30 or 31
    // then theres a . or a slash, and we continue the same logic for the months and year
    static const std::regex pattern(R"(^(0[1-9]|[12][0-9]|3[01])[./](0[1-9]|1[0-2])[./]\d{4}$)");
    return std::regex_match(birthDate, pattern);
}