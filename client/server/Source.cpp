#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	//TriviaServer server;
	//server.serve();
	DB db;
	std::cout << db.isUserAndPassMatch("user", "Aa12") << db.isUserAndPassMatch("yolo", "Aa12");
	system("pause");
	return(0);
}