#pragma once

#include <boost\asio.hpp>
using boost::asio::ip::tcp;
class Helper
{
public:
	static void sendData(std::shared_ptr<tcp::socket>, std::string);
	static int getMessageTypeCode(std::shared_ptr<tcp::socket>);
	static int getIntPartFromSocket(std::shared_ptr<tcp::socket>, int);
	static std::string getStringPartFromSocket(std::shared_ptr<tcp::socket>, int);
	static std::string getPaddedNumber(int, int);
	static char* Helper::getPartFromSocket(std::shared_ptr<tcp::socket>, int);
};