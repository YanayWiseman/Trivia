#pragma once

#include <iostream>
#include <vector>

class Question
{
private:
	// fields
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	int m_correctAnswerId;
	std::string m_imageBase64;

public:
	// C'tor and D'tor
	Question(const std::string& qText, const std::vector<std::string>& answers, int correctAnsId, const std::string& imageBase64);
	~Question() = default;

	// getters
	int getCorrectAnswerId() const;
	std::string getQuestion() const;
	std::vector <std::string> getPossibleAnswers() const;
	std::string getImageBase64() const;
};