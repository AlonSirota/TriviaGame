#pragma once

#include "protocol.h"
#include "Helper.h"
#include "Game.h"
//#include "Room.h" this causes an error, because room uses user as well - do we really need this?
#include <iostream>
#include <boost\asio.hpp>
using boost::asio::ip::tcp;

#define USERERROR -1


class User
{
public:
	User(std::string, tcp::socket&);
	User(User&&);
	void send(std::string&);
	std::string getUsername() const;
	tcp::socket& getSocket() { return(_socket); }
	bool joinRoom(int);
	void leaveRoom();
	int closeRoom();
	void operator=(const User & other);
	bool leaveGame();

	int _currRoomID;
	int _currGameID;
private:
	std::string _username;
	tcp::socket &_socket;
};
