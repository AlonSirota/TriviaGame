#pragma once

#include <boost\asio.hpp>
using boost::asio::ip::tcp;
class Helper
{
public:
	static void sendData(tcp::socket*, std::string);
	static std::string getMessageTypeCode(tcp::socket*);
	static int getIntPartFromSocket(tcp::socket*, int);
	static std::string getStringPartFromSocket(tcp::socket*, int);
	static std::string getPaddedNumber(int, int);//not sure we'll need this one
	static char* Helper::getPartFromSocket(tcp::socket*, int);
};