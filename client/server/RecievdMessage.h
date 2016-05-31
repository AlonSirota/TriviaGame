#pragma once
#include "User.h"
class recievedMessage
{
public:
	recievedMessage(tcp::socket&, int,User&);
	recievedMessage(tcp::socket&, int, std::vector<std::string>, User&);
	recievedMessage();

	tcp::socket& _socket;
	User& _user;
	int _messageCode;
	std::vector<std::string> _values;
};