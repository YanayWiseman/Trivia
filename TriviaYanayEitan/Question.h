#pragma once

#include <iostream>
#include <vector>

class Question
{
private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	int m_correctAnswerId;

public:
	std::string getQuestion();
	std::vector <std::string> getPossibleAnswers();
	int getCorrectAnswerId();
};