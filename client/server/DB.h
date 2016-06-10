#pragma once
#include <string>
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include "Question.h"
class DB
{
public:
	DB();
	bool isUserExist(std::string);
	bool addNewUser(std::string, std::string, std::string);
	bool isUserAndPassMatch(std::string, std::string);
	std::vector<std::shared_ptr<Question>> questionInit(int);
	std::vector<std::string> getBestScores();
	std::vector<std::string> getPersonalStatus(std::string);
	int insertNewGame();
	bool updateGameStatus(int);
	bool addAnswerToUser(int, std::string, int, std::string, bool,int);
	void example();
private:
	SQLite::Database _db;
	enum _player_answer
	{
		_true = 1,
		_false = 0
	};
};
