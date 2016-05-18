#include "User.h"
//done
User::User(std::string username, tcp::socket* socket) : _username(username), _socket(socket)
{
	_currGame = nullptr;
	_currRoom = nullptr;
}

void User::send(std::string)
{

}

bool User::createRoom(int, std::string, int, int, int)
{

}
//done
bool User::joinRoom(Room* room)
{
	if (_currRoom == nullptr)
	{
		return(false);
	}
	else if (room->joinRoom(this))
	{
		_currRoom = room;
		return(true);
	}
	else
	{
		return(false);
	}
}
//done
void User::leaveRoom()
{
	if (_currRoom != nullptr)
	{
		_currRoom->leaveRoom(this);
		_currRoom = nullptr;
	}

}
//done
int User::closeRoom()
{
	if (_currRoom == nullptr)
	{
		return(-1);
	}
	int roomId = _currRoom->closeRoom(this);
	return(roomId);
}
//done
bool User::leaveGame()
{
	if (_currGame != nullptr)
	{
		_currGame->leaveGame(this);
		_currGame = nullptr;
	}
}
//done
void User::setGame(Game* game)
{
	_currGame = game;
	_currRoom = nullptr;
}
//done
void User::clearGame()
{
	_currGame = nullptr;
}

//getter

std::string User::getUsername()
{
	return(_username);
}
//done
tcp::socket* User::getSocket()
{
	return(_socket);
}
//done
Room* User::getRoom()
{
	return(_currRoom);
}
//done
Game* User::getGame()
{
	return(_currGame);
}