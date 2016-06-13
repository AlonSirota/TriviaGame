#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	TriviaServer server;
	server.serve();
	//DB db;
	//db.addNewUser("user", "Aa12", "debug@help.help");
	//db.insertQuestion("what color is the sky?", "blue", "red", "green", "yellow");
	//std::cout << "should print 0: " << db.addAnswerToUser(db.insertNewGame(), "alon", 1, "ans1", true, 3);

	system("pause");
	return(0);
}