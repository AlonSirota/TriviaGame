#pragma once
#include <iostream>

#include "TriviaServer.h"

int main()
{
	TriviaServer server;
	server.serve();

	system("pause");
	return(0);
}