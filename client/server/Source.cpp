#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	//TriviaServer server;
	//server.serve();
	DB db;
	std::vector<std::string> v = db.getPersonalStatus("moshe");
	for (int i = 0; i < 4; i++)
	{
		std::cout << v[i] << "\n";
	}

	system("pause");
	return(0);
}