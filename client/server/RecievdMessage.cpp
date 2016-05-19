#include "RecievdMessage.h"

recievedMessage::recievedMessage(tcp::socket* socket, int messageCode)
{
	_messageCode = messageCode;
	_socket = socket;
	
}

recievedMessage::recievedMessage(tcp::socket* socket, int messageCode, std::vector<std::string> values)
{
	_socket = socket;
	_messageCode = messageCode;
	_values = values;
}
