#include "Helper.h"

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <iomanip>

void Helper::sendData(tcp::socket& socket, std::string bufTemp)
{
	boost::asio::async_write(socket, boost::asio::buffer(bufTemp.c_str(), bufTemp.length()), [](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{

		}
	}); 
	
	//will send exception automaticly
}

int Helper::getMessageTypeCode(tcp::socket& socket)
{
	char* s = new char[3];
	s = getPartFromSocket(socket, 3);
	std::string msg(s);

	if (msg == "")
		return 0;
	return  atoi(s);
}

int Helper::getIntPartFromSocket(tcp::socket& socket, int bytesNum)
{
	char* s = getPartFromSocket(socket, bytesNum);
	return atoi(s);
}

std::string Helper::getStringPartFromSocket(tcp::socket& socket, int bytesNum)
{
	char* s = getPartFromSocket(socket, bytesNum);
	std::string res(s);
	return res;
}

std::string Helper::getPaddedNumber(int num, int digits)
{
	std::ostringstream ostr;
	ostr << std::setw(digits) << std::setfill('0') << num;
	return ostr.str();
}

char * Helper::getPartFromSocket(tcp::socket& socket, int bytesNum)
{
	if (bytesNum == 0)
	{
		return "";
	}

	//char* data = new char[bytesNum + 1];
	std::vector<char> data(bytesNum + 1);
	socket.async_read_some(boost::asio::buffer(data.data(), data.size()), [](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{

		}
	});
	data[bytesNum] = NULL;
	return data.data();
}
