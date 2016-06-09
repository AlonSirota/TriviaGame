#include <vector>

class Question
{
	Question(int, std::string, std::string, std::string, std::string, std::string);
	std::string getQuestion();
	std::vector<std::string> getAnswers();
	int getCorrectAnswerIndex();
	int getId();
private:
	std::string _question;
	std::vector<std::string> _answers;
	int _correctAnswerIndex;
	int _id;
};