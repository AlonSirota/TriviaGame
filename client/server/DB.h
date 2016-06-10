#pragma once
#include <string>
#include "include\SQLiteCpp\SQLiteCpp.h"
//#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <iostream>

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
	//private variables
};
