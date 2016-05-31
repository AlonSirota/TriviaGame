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

recievedMessage::recievedMessage()
{

}

