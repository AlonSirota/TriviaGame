#pragma once
#include "User.h"
class recievedMessage
{
public:
	recievedMessage(std::shared_ptr<tcp::socket>, int,User&);
	recievedMessage(std::shared_ptr<tcp::socket>, int, std::vector<std::string>, User&);
	recievedMessage(const recievedMessage&);

	std::shared_ptr<tcp::socket> _socket;
	User& _user;
	int _messageCode;
	std::vector<std::string> _values;
};