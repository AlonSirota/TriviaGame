#pragma once

#include "Helper.h"
#include "Game.h"
#include "Room.h"
#include <iostream>
#include <boost\asio.hpp>
using boost::asio::ip::tcp;

#define USERERROR -1
#define CREATEROOMFAILED "1140"
#define CREATEROOMSUCSESS "1141"

class User
{
	User(std::string, tcp::socket*);
	void send(std::string&);
	std::string getUsername() { return(_username); }
	tcp::socket* getSocket() { return(_socket); }
	Room* getRoom() { return(_currRoom); }
	Game* getGame() { return(_currGame); }
	void setGame(Game*);
	void clearGame() { _currGame = nullptr; }
	bool createRoom(int, std::string, int, int, int);
	bool joinRoom(Room*);
	void leaveRoom();
	int closeRoom();
	bool leaveGame();
private:
	std::string _username;
	Room* _currRoom;
	Game* _currGame;
	tcp::socket* _socket;
};
