#include "User.h"
//done
User::User(std::string username, tcp::socket &socket) : _username(username), _socket(std::move(socket))
{
	_currGameID = 0;
	_currRoomID = 0;
}

User::User(User &&other) : _socket(std::move(other._socket)), _username(other._username)
{
	this->_currGameID = other._currGameID;
	this->_currRoomID = other._currRoomID;
}

void User::send(std::string& buffer)
{
	Helper::sendData(_socket, buffer);
}
/*
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
	
}*/
//done

bool User::joinRoom(int roomId)
{
	if (_currRoomID == 0)
	{
		return(false);
	}
	_currRoomID = roomId;
	return(true);
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
	int temp = _currRoomID;
	_currRoomID = 0;
	return(temp);
}

void User::operator=(const User & other)
{
	this->_currGameID = other._currGameID;
	this->_currRoomID = other._currRoomID;
	this->_socket = tcp::socket(std::move(other._socket));
	this->_username = other._username;
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