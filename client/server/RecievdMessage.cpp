#include "RecievdMessage.h"

recievedMessage::recievedMessage(std::shared_ptr<tcp::socket> socket, int messageCode, User& user) :_socket(socket), _user(user)
{
	_messageCode = messageCode;
}

recievedMessage::recievedMessage(std::shared_ptr<tcp::socket> socket, int messageCode, std::vector<std::string> values, User& user) : _socket(socket), _user(user)
{
	_messageCode = messageCode;
	_values = values;
}
/*
recievedMessage::recievedMessage(const recievedMessage &other ) : _messageCode(other._messageCode), _socket(std::move(other._socket)), _user(other._user), _values(other._values) //TODO should we use std::move?
{
}*/

recievedMessage::recievedMessage(recievedMessage &other) : _messageCode(other._messageCode), _socket(other._socket), _user(other._user), _values(other._values) //TODO should we use std::move?
{
}