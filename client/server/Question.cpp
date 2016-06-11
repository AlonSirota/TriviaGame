#include "Question.h"

Question::Question(int id, std::string question, std::string correctAns, std::string ans1, std::string ans2, std::string ans3) : _answers(4)
{
	_id = id;
	_question = question;
	bool taken[4] = { false,false,false,false };
	srand(0);//fix this
	//correct ans
	_correctAnswerIndex = rand() % 4;//check this
	_answers[_correctAnswerIndex] = correctAns;
	taken[_correctAnswerIndex] = true;
	//first ans
	int temp = rand() % 4;
	while (taken[temp])
	{
		temp = rand() % 4;
	}
	_answers[temp] = ans1;
	taken[temp] = true;
	//second ans
	temp = rand() % 4;
	while (taken[temp])
	{
		temp = rand() % 4;
	}
	_answers[temp] = ans2;
	taken[temp] = true;
	//third ans
	temp = rand() % 4;
	while (taken[temp])
	{
		temp = rand() % 4;
	}
	_answers[temp] = ans3;
	taken[temp] = true;
}

std::string Question::getQuestion()
{
	return _question;
}

std::vector<std::string> Question::getAnswers()
{
	return _answers;
}

int Question::getCorrectAnswerIndex()
{
	return _correctAnswerIndex;
}

int Question::getId()
{
	return _id;
}

std::string Question::toString()
{
	std::string str;
	str.append("id: " + std::to_string(_id) + "\n" + 
		"question: " + _question + "\n" +
		"correct answer id: " + std::to_string(_correctAnswerIndex) + "\n");

	for (int i = 0; i < 4; i++)
	{
		str.append("ans " + std::to_string(i) + ": " + _answers[i] + "\n");
	}

	return str;
}
