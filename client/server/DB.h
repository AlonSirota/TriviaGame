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
	std::vector<std::pair<std::string, std::string>> DB::getBestScores(int amount);
	std::vector<std::string> getPersonalStatus(std::string);
	int insertNewGame();
	bool updateGameStatus(int);
	bool addAnswerToUser(int, std::string, int, std::string, bool,int);
	void example();
	static std::string columnToString(SQLite::Column c);

	static enum _personalStatusIndexes
	{
		NUMBER_OF_GAMES = 0,
		NUMBER_OF_RIGHT_ANWERS = 1,
		NUMBER_OF_WRONG_ANSWERS = 2,
		AVG_TIME_FOR_ANS = 3
	};

	static enum _player_answer
	{
		_true = 1,
		_false = 0
	};
private:
	SQLite::Database _db;
};
