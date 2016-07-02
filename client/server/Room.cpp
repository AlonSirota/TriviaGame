#include "Room.h"
Room::Room(int id, std::shared_ptr<User> admin, std::string roomName, int maxUsers, int questionsNo, int questionTime) : _admin(admin), _name(roomName)
{
	_id = id;
	_maxUsers = maxUsers;
	_questionsNo = questionsNo;
	_questionTime = questionTime;
	_users.push_back(_admin);
}

void Room::joinRoom(std::shared_ptr<User> user)
{
	std::string reply = std::to_string(JOIN_ROOM_REPLY);
	if (_users.size() >= _maxUsers)
	{
		user->send(reply + "1"); //1 means room is full.
	}
	else
	{
		_users.push_back(user);//add him
		user->send(reply + "0" + Helper::getPaddedNumber(_questionsNo,2)+ Helper::getPaddedNumber(_questionTime, 2));
		this->sendMessage(this->getUsersListMessage()); //send everyone the current list.
	}
}

void Room::leaveRoom(std::shared_ptr<User> user)
{
			_users.erase(std::find(_users.begin(), _users.end(), user));
			user->send(std::to_string(LEAVE_ROOM_REPLY));
			this->sendMessage(this->getUsersListMessage()); //sends after user was delted from user vector - so the leaving user wont get the message.
}

int Room::closeRoom(std::shared_ptr<User> caller, bool startGame)
{
	int id;
	if (caller != _admin)
	{
		return -1;
	}
	id = _admin->_currRoomID;

	if (!startGame)	this->sendMessage(std::to_string(CLOSE_ROOM_REPLY)); //only sends if the admin closed the room (not because he started a game).
	
	std::for_each(_users.begin(), _users.end(), [](std::shared_ptr<User> currUser)
	{
		currUser->_currRoomID = 0;
	});
	
	return id;
}

std::string Room::getUsersAsString(std::vector<std::shared_ptr<User>> usersVector, std::shared_ptr<User> excluded)
{
	std::string list;
	for (int i = 0; i < usersVector.size(); i++)
	{
		list.append(usersVector[i]->_username);
		if (i == usersVector.size() - 1)
		{
			list.append(",");
		}
	}
	return list;
}

std::string Room::getUsersListMessage()
{
	std::string message = std::to_string(USERS_IN_ROOM_REPLY);
	int size = _users.size();
	message.append(std::to_string(size)); //appends number of users.
	for (int i = 0; i < size; i++)
	{
		message.append(Helper::getPaddedNumber(this->_users[i]->_username.length(), 2));
		//message.append(std::to_string(this->_users[i]->_username.length())); //appends the size of the current
		message.append(this->_users[i]->_username);
	}

	return message;
}

void Room::sendMessage(std::string message)
{
	for (int i = 0; i < _users.size(); i++)
	{
		_users[i]->send(message);
	}
}
