#pragma once
#include <vector>
#include <string>

class Question
{
public:
	Question(int id, std::string question, std::string correctAns, std::string ans1, std::string ans2, std::string ans3);
	std::string getQuestion();
	std::vector<std::string> getAnswers();
	int getCorrectAnswerIndex();
	int getId();
	std::string toString();
private:
	std::string _question;
	std::vector<std::string> _answers;
	int _correctAnswerIndex;
	int _id;
};