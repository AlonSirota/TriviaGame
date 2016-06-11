#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	//TriviaServer server;
	//server.serve();
	DB db;
	try
	{
		std::vector<std::shared_ptr<Question>> q = db.questionInit(3);
		std::cout << q[0]->toString() << q[1]->toString() << q[2]->toString();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	system("pause");
	return(0);
}