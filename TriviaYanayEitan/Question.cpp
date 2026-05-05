#include "Question.h"


/*
    Function returns question string.
    Input: None
    Output: Question (std::string)
*/

std::string Question::getQuestion()
{
    return this->m_question;
}


/*
    Function returns vector of possible answers string.
    Input: None
    Output: Possible answer strings (std::string)
*/

std::vector<std::string> Question::getPossibleAnswers()
{
    return this->m_possibleAnswers;
}


/*
    Function returns index of correct answer.
    Input: None
    Output: Correct answer index (int)
*/

int Question::getCorrectAnswerId()
{
    return this->m_correctAnswerId;
}




