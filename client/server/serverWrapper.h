#pragma once
#include <boost\asio.hpp>
#include <iostream>
#include <boost\bind.hpp>

class serverWrapper {
private:
	boost::asio::io_service _io;
	boost::asio::ip::tcp::socket _listenSocket;
	boost::asio::ip::tcp::acceptor _acceptor;
	int _port;
public:
	serverWrapper(int port) : _listenSocket(_io), _acceptor(_io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	{
		_port = port;
	}

	~serverWrapper();

	void run();
	void acceptHandler(const boost::system::error_code& e, boost::asio::ip::tcp::socket& s);
	static void sendHandler(const boost::system::error_code& e);
	static void readHandler(const boost::system::error_code& e);
	static void mySend(boost::asio::ip::tcp::socket& s, std::string str);
	static std::string myReceive(boost::asio::ip::tcp::socket& s);
};