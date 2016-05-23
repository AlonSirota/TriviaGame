#pragma once

#include <boost\asio.hpp>
using boost::asio::ip::tcp;
class Helper
{
public:
	static void sendData(tcp::socket*, std::string);
	static int getMessageTypeCode(tcp::socket*);
	static int getIntPartFromSocket(tcp::socket*, int);
	static std::string getStringPartFromSocket(tcp::socket*, int);
	static std::string getPaddedNumber(int, int);]
	static char* Helper::getPartFromSocket(tcp::socket*, int);
};