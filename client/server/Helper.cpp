#include "Helper.h"

void Helper::sendData(tcp::socket* socket, std::string bufTemp)
{
	boost::asio::write(*socket, boost::asio::buffer(bufTemp.c_str(), bufTemp.length())); //change to async
}
