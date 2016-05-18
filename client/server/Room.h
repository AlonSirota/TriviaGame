#pragma once

//#include "User.h"
#include <vector>
class User;

class Room
{
public:
	Room(int, User*, std::string, int, int, int);
	bool joinRoom(User*);
	void leaveRoom(User*);
	int closeRoom(User*);
	std::vector<User*> getUsers();
	std::string getUsersListMessage();
	int getQuestionsNo();
	int getId();
	std::string getName();
private:
	//add private functions and variables
};
