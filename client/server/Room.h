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
	std::vector<User*> _users;
	User* _admin;
	int _maxUsers;
	int _questionsNo;
	int _questionTime;
	std::string _name;
	int _id;
	std::string getUsersAsString(std::vector<User*>, User*);
	void sendMessage(std::string);
	void sendMessage(User*, std::string);
};
