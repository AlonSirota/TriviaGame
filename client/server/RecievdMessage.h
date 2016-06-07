#pragma once
#include "User.h"
class recievedMessage
{
public:
	recievedMessage(std::shared_ptr<tcp::socket>, int, std::shared_ptr<User>);
	recievedMessage(std::shared_ptr<tcp::socket>, int, std::vector<std::string>, std::shared_ptr<User>);
	recievedMessage(const recievedMessage&);

	std::shared_ptr<tcp::socket> _socket;
	std::shared_ptr<User> _user;
	int _messageCode;
	std::vector<std::string> _values;
};