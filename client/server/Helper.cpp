#include "Helper.h"

void Helper::sendData(tcp::socket* socket, std::string bufTemp)
{
	//boost::asio::const_buffer buffer =  boost::asio::buffer(bufTemp);
	//auto self(shared_from_this());
	boost::asio::write(socket, boost::asio::buffer(bufTemp.c_str(), bufTemp.length())); //ERROR FIND SOLUTION
	/*boost::asio::write(socket, boost::asio::buffer(bufTemp.c_str(), bufTemp.length()));/*, //change to async
		//[](boost::system::error_code /*ec*///, std::size_t /*length*/)
	//{
	//});
}
