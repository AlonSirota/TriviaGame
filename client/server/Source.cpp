#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	//TriviaServer server;
	//server.serve();
	DB db;
	std::vector<std::pair<std::string, std::string>> v = db.getBestScores(3);
	for (int i = 0; i < v.size(); i++)
	{
		std::cout << v[i].first << " " << v[i].second << "\n";
	}

	system("pause");
	return(0);
}