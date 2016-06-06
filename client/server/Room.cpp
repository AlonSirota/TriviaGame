#include "Room.h"
Room::Room(int id, User &admin, std::string roomName, int maxUsers, int questionsNo, int questionTime) : _admin(admin), _name(roomName)
{
	_id = id;
	_maxUsers = maxUsers;
	_questionsNo = questionsNo;
	_questionTime = questionTime;
	_users.push_back(_admin);
}

void Room::joinRoom(User &&user)
{
	_users.push_back(std::move(user));
}

void Room::leaveRoom(User &user)
{

}

int Room::closeRoom(User)
{
	return 0;
}

std::string Room::getUsersAsString(std::vector<User> usersVector, User excluded)
{
	std::string list;
	for (int i = 0; i < usersVector.size(); i++)
	{
		list.append(usersVector[i]._username);
		if (i == usersVector.size() - 1)
		{
			list.append(",");
		}
	}
	return list;
}

std::string Room::getUsersListMessage()
{
	std::string message = "108";
	int size = _users.size();
	message.append(std::to_string(size)); //appends number of users.
	for (int i = 0; i < size; i++)
	{
		message.append(std::to_string(this->_users[i]._username.length())); //appends the size of the current
		message.append(this->_users[i]._username);
	}

	return message;
}

void Room::sendMessage(std::string message)
{
	for (int i = 0; i < _users.size(); i++)
	{
		_users[i].send(message);
	}
}
