#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	//TriviaServer server;
	//server.serve();
	DB db;
	std::cout << db.addNewUser("user", "Aa12", "email");
	system("pause");
	return(0);
}