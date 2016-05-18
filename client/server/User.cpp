#include "User.h"
//done
User::User(std::string username, tcp::socket socket) : _username(username), _socket(socket)
{
	_currGame = nullptr;
	_currRoom = nullptr;
}

void User::send(std::string)
{

}
//done
void User::setGame(Game* game)
{
	_currGame = game;
	_currRoom = nullptr;
}
//check
void User::clearRoom()
{
	_currGame = nullptr;
}

bool User::createRoom(int, std::string, int, int, int)
{

}

bool User::joinRoom(Room*)
{

}

void User::leaveRoom()
{

}

int User::closeRoom()
{

}

bool User::leaveGame()
{

}


//getter

std::string User::getUsername()
{
	return(_username);
}

tcp::socket User::getSocket()
{
	return(_socket);
}

Room* User::getRoom()
{
	return(_currRoom);
}

Game* User::getGame()
{
	return(_currGame);
}