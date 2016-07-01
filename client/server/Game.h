#pragma once
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
	void sendStartGameMessageToAllUsers();
	void sendMessageToAllUsers(std::string message);

	int _questionsNo;
	std::vector<std::shared_ptr<Question>> _questions;
	std::vector<std::shared_ptr<User>> _users;

	std::shared_ptr<DB> _db;
	std::map<std::string, int> _results;
	int _answersRegisteredThisTurnCount;
	int _id;

private:
	bool insertGameToDB();
	void initQuestionFromDB();
	int _currQuestionIndex;	
};
