#include "Room.h"

Room::Room(int, User *, std::string, int, int, int)
{
}

bool Room::joinRoom(User *)
{
	return false;
}

void Room::leaveRoom(User *)
{
}

int Room::closeRoom(User *)
{
	return 0;
}

std::vector<User*> Room::getUsers()
{
	return std::vector<User*>();
}

std::string Room::getUsersListMessage()
{
	return std::string();
}

int Room::getQuestionsNo()
{
	return 0;
}

int Room::getId()
{
	return 0;
}

std::string Room::getName()
{
	return std::string();
}
