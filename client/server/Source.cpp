#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	//TriviaServer server;
	//server.serve();
	DB db;
	std::cout << db.questionInit(1)[0];
	system("pause");
	return(0);
}