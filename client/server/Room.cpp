#include "Room.h"
Room::Room(int id, User &admin, std::string roomName, int maxUsers, int questionsNo, int questionTime) : _admin(admin), _name(roomName)
{
	_id = id;
	_maxUsers = maxUsers;
	_questionsNo = questionsNo;
	_questionTime = questionTime;
}

void Room::joinRoom(User &&user)
{
	_users.push_back(user);
}

void Room::leaveRoom(User &user)
{

}

int Room::closeRoom(User)
{
	return 0;
}


std::string Room::getUsersListMessage()
{
	return std::string();
}

void Room::sendMessage(std::string)
{
}

void Room::sendMessage(User&, std::string)
{
}
