#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	//TriviaServer server;
	//server.serve();
	DB db;
	std::cout << db.isUserExist("user");
	system("pause");
	return(0);
}