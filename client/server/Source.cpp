#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	//TriviaServer server;
	//server.serve();
	DB db;
	int id = db.insertNewGame();
	std::cout << db.updateGameStatus(id);

	system("pause");
	return(0);
}