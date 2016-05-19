#pragma once

#include <boost\asio.hpp>
using boost::asio::ip::tcp;
class Helper
{
public:
	static void sendData(tcp::socket*, std::string);
};