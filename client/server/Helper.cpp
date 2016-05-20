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
}
