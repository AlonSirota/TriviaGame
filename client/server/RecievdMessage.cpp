#include "RecievdMessage.h"

recievedMessage::recievedMessage(tcp::socket& socket, int messageCode, User& user) :_socket(socket), _user(user)
{
	_messageCode = messageCode;
}

recievedMessage::recievedMessage(tcp::socket& socket, int messageCode, std::vector<std::string> values, User& user) : _socket(socket), _user(user)
{
	_messageCode = messageCode;
	_values = values;
}

recievedMessage::recievedMessage(const recievedMessage &other ) : _messageCode(other._messageCode), _socket(other._socket), _user(other._user), _values(std::move(other._values))
{
}