#include "serverWrapper.h"

serverWrapper::~serverWrapper()
{
	//TODO
}

void serverWrapper::run()
{
	boost::asio::ip::tcp::socket s(_io);

	_acceptor.async_accept(s, boost::bind(&serverWrapper::acceptHandler, this, boost::asio::placeholders::error, std::ref(s))); //works
	_io.run();
	//TODO handle failure
}

void serverWrapper::acceptHandler(const boost::system::error_code& e, boost::asio::ip::tcp::socket& s)
{
	if (e)
	{
		std::cout << "async_accept failed, error code: " << e.value();
	}
	else
	{
		std::cout << myReceive(s);
		mySend(s, "sent by server");
		s.close();

		boost::asio::ip::tcp::socket newSocket(_io);
		_acceptor.async_accept(newSocket, boost::bind(&serverWrapper::acceptHandler, this, boost::asio::placeholders::error, std::ref(newSocket)));
	}
}

void serverWrapper::sendHandler(const boost::system::error_code & e)
{
	if (e)
	{
		std::cout << "sendHandler invoked, error: " << e.value();
		//TODO
	}
}

void serverWrapper::readHandler(const boost::system::error_code & e)
{
	if (e)
	{
		std::cout << "readHandler invoked, error: " << e.value();
		//TODO handle error
	}
}

void serverWrapper::mySend(boost::asio::ip::tcp::socket& s, std::string str)
{
	s.send(boost::asio::buffer(str)); //TODO make this async
									  //_listenSocket.async_write_some(boost::asio::buffer(str), boost::bind(&serverWrapper::sendHandler, this, boost::asio::placeholders::error));
}

std::string serverWrapper::myReceive(boost::asio::ip::tcp::socket& s) //works
{
	char received[1024];
	s.async_read_some(boost::asio::buffer(received), boost::bind(&serverWrapper::readHandler, boost::asio::placeholders::error));
	return received;
}
