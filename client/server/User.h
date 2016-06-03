#pragma once

#include "protocol.h"
#include "Helper.h"
#include "Game.h"
#include "Room.h"
#include <iostream>
#include <boost\asio.hpp>
using boost::asio::ip::tcp;

#define USERERROR -1


class User
{
public:
	User(std::string, tcp::socket);
	void send(std::string&);
	std::string getUsername() { return(_username); }
	tcp::socket& getSocket() { return(_socket); }
	bool joinRoom(int);
	void leaveRoom();
	int closeRoom();
	bool leaveGame();

	int _currRoomID;
	int _currGameID;
private:
	std::string _username;
	tcp::socket _socket;
};
