#pragma once

#include "protocol.h"
#include "Helper.h"
#include "Game.h"
//#include "Room.h" this causes an error, because room uses user as well - do we really need this? YES WE DO
#include <iostream>
#include <boost\asio.hpp>
using boost::asio::ip::tcp;

#define USERERROR -1


class User
{
public:
	User(std::string, std::shared_ptr<tcp::socket>);
	User(User&&);
	User(const User&);
	void send(std::string&);
	std::string getUsername() const;
	std::shared_ptr<tcp::socket> getSocket() { return(_socket); }
	bool joinRoom(int);
	void leaveRoom();
	void operator=(const User & other);
	bool operator==(const User & other) const;
	bool operator<(const User& other) const;
	bool leaveGame();

	int _currRoomID;
	int _currGameID;
	std::string _username;
	std::shared_ptr<tcp::socket> _socket;	
};
