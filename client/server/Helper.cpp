#include "Helper.h"

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <iomanip>

void Helper::sendData(std::shared_ptr<tcp::socket> socket, std::string bufTemp)
{
	boost::asio::async_write(*socket, boost::asio::buffer(bufTemp.c_str(), bufTemp.length()), [](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{

		}
	}); 
	
	//will send exception automaticly
}

int Helper::getMessageTypeCode(std::shared_ptr<tcp::socket> socket)
{
	std::vector<char> s = getPartFromSocket(socket, 3);
	std::string msg(s.data());

	if (msg == "")
		return 0;
	std::string data = msg.substr(0,3);
	return  atoi(data.c_str());
}

int Helper::getIntPartFromSocket(std::shared_ptr<tcp::socket> socket, int bytesNum)
{
	std::vector<char> data(getPartFromSocket(socket, bytesNum));
	//char* s = data.data();
	return atoi(data.data());
}

std::string Helper::getStringPartFromSocket(std::shared_ptr<tcp::socket> socket, int bytesNum)
{
	std::vector<char> s = getPartFromSocket(socket, bytesNum);
	std::string res(s.data());
	return res;
}

std::string Helper::getPaddedNumber(int num, int digits)
{
	std::ostringstream ostr;
	ostr << std::setw(digits) << std::setfill('0') << num;
	return ostr.str();
}

std::vector<char> Helper::getPartFromSocket(std::shared_ptr<tcp::socket> socket, int bytesNum)
{
	if (bytesNum == 0)
	{
		return std::vector<char>();
	}
	std::vector<char> data(bytesNum);
	socket->read_some(boost::asio::buffer(data.data(), data.size()));

	//socket->async_read_some(boost::asio::buffer(data.data(), data.size()), [](boost::system::error_code ec, std::size_t /*length*/)
	//{
	//	if (!ec)
//		{

		//}
	//});
	data.push_back(NULL);
	return data;
}
