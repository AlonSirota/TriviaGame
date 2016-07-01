#pragma once
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <iomanip>
#include <boost\asio.hpp>
//#include <boost\asio\ssl.hpp>
using boost::asio::ip::tcp;
class Helper
{
public:
	static void sendData(std::shared_ptr<tcp::socket>, std::string, bool encrypted);
	static int getMessageTypeCode(std::shared_ptr<tcp::socket>, bool encrypted);
	static int getIntPartFromSocket(std::shared_ptr<tcp::socket>, int, bool encrypted);
	static std::string getStringPartFromSocket(std::shared_ptr<tcp::socket>, int, bool encrypted);
	static std::string getPaddedNumber(int, int);
	static std::vector<char> Helper::getPartFromSocket(std::shared_ptr<tcp::socket>, int, bool encrypted);
};