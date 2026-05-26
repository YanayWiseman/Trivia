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
	Question(const std::string& qText, const std::vector<std::string>& answers, int correctAnsId);
	~Question() = default;
	int getCorrectAnswerId() const;
	std::string getQuestion() const;
	std::vector <std::string> getPossibleAnswers() const;
};