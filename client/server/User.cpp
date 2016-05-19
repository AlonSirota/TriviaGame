#include "User.h"
//done
User::User(std::string username, tcp::socket* socket) : _username(username), _socket(socket)
{
	_currGame = nullptr;
	_currRoom = nullptr;
}

void User::send(std::string& buffer)
{
	Helper::sendData(_socket, buffer);
}

bool User::createRoom(int roomId, std::string roomName, int maxUsers, int questionsNo, int questionTime)
{
	if (_currRoom != nullptr)
	{
		std::string message = CREATEROOMFAILED; // may create problems as it may delete itself
		this->send(message);
		return(false);
	}
	_currRoom = new Room(roomId, this, roomName, maxUsers, questionsNo, questionTime);
	if (_currRoom == nullptr)
	{
		std::string message = CREATEROOMFAILED; // may create problems as it may delete itself
		this->send(message);
		return(false);
		//send failed message 1140
	}
	//send sucsess message 1141
	std::string message = CREATEROOMSUCSESS; // may create problems as it may delete itself
	this->send(message);
	return(true);
	
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
		return(USERERROR);
	}
	int roomId = _currRoom->closeRoom(this);
	if (roomId == USERERROR)
	{
		return(USERERROR);
	}
	else
	{
		delete _currRoom;
	}
	return(roomId);
}
//done
bool User::leaveGame()
{
	if (_currGame != nullptr)
	{
		bool ans = _currGame->leaveGame(this);
		_currGame = nullptr;
		return(ans);
	}
	return(false);
}
//done
void User::setGame(Game* game)
{
	_currGame = game;
	_currRoom = nullptr;
}
