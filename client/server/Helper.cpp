#include "Helper.h"

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>

void Helper::sendData(tcp::socket* socket, std::string bufTemp)
{
	boost::asio::async_write(*socket, boost::asio::buffer(bufTemp.c_str(), bufTemp.length()), [](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{

		}
	});
	//will send exception automaticly
}

std::string Helper::getMessageTypeCode(tcp::socket* socket)
{
	char* s = new char[3];
	s = getPartFromSocket(socket, 3);
	std::string msg(s);

	if (msg == "")
		return 0;
	return  s;
}

int Helper::getIntPartFromSocket(tcp::socket* socket, int bytesNum)
{
	char* s = getPartFromSocket(socket, bytesNum);
	return atoi(s);
}

std::string Helper::getStringPartFromSocket(tcp::socket* socket, int bytesNum)
{
	char* s = getPartFromSocket(socket, bytesNum);
	std::string res(s);
	return res;
}

char * Helper::getPartFromSocket(tcp::socket * socket, int bytesNum)
{
	if (bytesNum == 0)
	{
		return "";
	}

	char* data = new char[bytesNum + 1];
	socket->async_read_some(data, [](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{

		}
	});
	return data;
}
