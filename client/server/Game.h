#pragma once
//#include "Question.h"
#include <map>
#include <memory>
#include "DB.h"
#include "User.h"

class Game
{
public:
	Game(std::vector<std::shared_ptr<User>>,int,std::shared_ptr<DB>);
	void handleFinishGame();
	bool handleNextTurn();
	bool handleAnswerFromUser(std::shared_ptr<User>, int, int);
	bool leaveGame(std::shared_ptr<User>);
	void sendQuestionToAllUsers();
private:
	int _questionsNo;
	std::vector<std::shared_ptr<Question>> _questions;
	std::vector<std::shared_ptr<User>> _users;
	int _currQuestionIndex;
	std::shared_ptr<DB> _db;
	std::map<std::string, int> _results;
	int _currentTurnAnswers;
	int _id;

	bool insertGameToDB();
	void initQuestionFromDB();
	
};
