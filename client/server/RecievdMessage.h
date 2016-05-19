#pragma once

//#include <vector>
#include "User.h"
class recievedMessage
{
public:
	recievedMessage(tcp::socket*, int);
	recievedMessage(tcp::socket*, int,std::vector<std::string>);
	tcp::socket* getSocket() { return(_socket); }
	User* getUser() { return(_user); }
	void setUser(User* user) { _user = user; }
	int getMessageCode() { return(_messageCode); }
	std::vector<std::string>& getValues() { return(_values); }

private:
	tcp::socket* _socket;
	User* _user;
	int _messageCode;
	std::vector<std::string> _values;
};