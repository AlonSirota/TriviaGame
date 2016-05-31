#include "User.h"
//done
User::User(std::string username, tcp::socket& socket) : _username(username), _socket(socket)
{
	_currGameID = 0;
	_currRoomID = 0;
}

void User::send(std::string& buffer)
{
	Helper::sendData(_socket, buffer);
}

bool User::createRoom(int roomId, std::string roomName, int maxUsers, int questionsNo, int questionTime)
{
	if (_currRoomID != 0)
	{
		std::string message = std::to_string(CREATE_ROOM_FAILED); // may create problems as it may delete itself
		send(message);
		return(false);
	}
	_currRoom = new Room(roomId, this, roomName, maxUsers, questionsNo, questionTime);
	if (_currRoom == nullptr)
	{
		std::string message = std::to_string(CREATE_ROOM_FAILED); // may create problems as it may delete itself
		send(message);
		return(false);
		//send failed message 1140
	}
	//send sucsess message 1141
	std::string message = std::to_string(CREATE_ROOM_SUCSESS); // may create problems as it may delete itself
	send(message);
	return(true);
	
}
//done
bool User::joinRoom(Room& room)
{
	if (_currRoomID == 0)
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
	_currRoomID = 0;

}
//done
int User::closeRoom()
{
	if (_currRoomID == 0)
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

void User::copy(const User &other)
{
	_username = other._username;
	_socket = std::move(other._socket); //have i used this properly
	_currRoomID = other._currRoomID;
	_currGameID = other._currGameID;
}

//done
/*
bool User::leaveGame()
{
	if (_currGameID != 0)
	{
		std::map<int, Game&> gameList = 
		Game& currGame = 
		bool ans = currGame.leaveGame(this);
		return(ans);
	}
	return(false);
}
*/