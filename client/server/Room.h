#pragma once

#include "User.h"
#include <vector>
class User;

class Room
{
public:
	Room(int, std::shared_ptr<User>, std::string, int, int, int);
	void joinRoom(std::shared_ptr<User>);
	void leaveRoom(std::shared_ptr<User>);
	int closeRoom(std::shared_ptr<User>);
	std::string getUsersAsString(std::vector<std::shared_ptr<User>>, std::shared_ptr<User>);
	void sendMessage(std::string);
	//void sendMessage(User&, std::string);
	std::string getUsersListMessage();

	std::vector<std::shared_ptr<User>> _users;
	std::shared_ptr<User> _admin;
	int _maxUsers;
	int _questionsNo;
	int _questionTime;
	std::string _name;
	int _id;	
};
