#include "Game.h"
#include "Room.h"
#include <iostream>
#include 
using boost::asio::ip::tcp;

class User
{
	User(std::string, tcp::socket);
	void send(std::string);
	std::string getUsername();
	tcp::socket getSocket();
	Room* getRoom();
	Game* getGame();
	void setGame(Game*);
	void clearRoom();
	bool createRoom(int, std::string, int, int, int);
	bool joinRoom(Room*);
	void leaveRoom();
	int closeRoom();
	bool leaveGame();
private:
	std::string _username;
	Room* _currRoom;
	Game* _currGame;
	tcp::socket _socket;
};