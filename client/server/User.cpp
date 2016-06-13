#include "User.h"
//done
User::User(std::string username, std::shared_ptr<tcp::socket> socket) : _username(username), _socket(socket)
{
	_currGameID = 0;
	_currRoomID = 0;
}

User::User(User &&other) : _socket(other._socket), _username(other._username)
{
	this->_currGameID = other._currGameID;
	this->_currRoomID = other._currRoomID;
}

User::User(const User &other) : _username(other._username), _socket(other._socket)
{
	_currRoomID = other._currRoomID;
	_currGameID = other._currGameID;
}

void User::send(std::string& buffer)
{
	Helper::sendData(_socket, buffer);
}
std::string User::getUsername() const
{
	return _username;
}

//done
bool User::joinRoom(int roomId)
{
	if (_currRoomID != 0)
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

/*
if (_currRoomID == 0) //is the default value of a room when it's created.
{
return(USERERROR);
}

int temp = _currRoomID;
_currRoomID = 0;
return(temp);*/

void User::operator=(const User & other)
{
	this->_currGameID = other._currGameID;
	this->_currRoomID = other._currRoomID;
	this->_socket = other._socket;
	this->_username = other._username;
}

bool User::operator==(const User & other) const
{
	if (this->_currGameID == other._currGameID &&
		this->_currRoomID == other._currRoomID &&
		this->_socket == other._socket &&
		this->_username == other._username)
	{
		return true;
	}
	return false;
}

bool User::operator!=(const User & other) const
{
	return !((*this) == other);
}

bool User::operator<(const User & other) const
{
	return _username < other._username;
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