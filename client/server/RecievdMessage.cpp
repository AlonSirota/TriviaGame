#include "RecievdMessage.h"

recievedMessage::recievedMessage(std::shared_ptr<tcp::socket> socket, int messageCode, std::shared_ptr<User> user) :_socket(socket), _user(user)
{
	_messageCode = messageCode;
}

recievedMessage::recievedMessage(std::shared_ptr<tcp::socket> socket, int messageCode, std::vector<std::string> values, std::shared_ptr<User> user) : _socket(socket), _user(user)
{
	_messageCode = messageCode;
	_values = values;
}

recievedMessage::recievedMessage(const recievedMessage &other) : _messageCode(other._messageCode), _socket(other._socket), _user(other._user), _values(other._values)
{

}

std::string recievedMessage::toString()
{
	std::string str, data;
	str.append(this->_user->_username + ": " + std::to_string(this->_messageCode) + " - ");
	for (int i = 0; i < this->_values.size(); i++)
	{
		data.append(_values[i]);
	}
	str.append(data + "\n");

	return str;
}
