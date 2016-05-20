#include "TriviaServer.h"

TriviaServer::TriviaServer()//:_db() - only in later version
{
	_socket = new tcp::socket(_io_service);
}















//done
User * TriviaServer::getUserByName(std::string username)
{
	bool found = false;
	std::map<tcp::socket*, User*>::iterator it = _connectedUsers.begin();
	while (it != _connectedUsers.end())
	{
		if (it->second->getUsername() == username)
			return(it->second);
		it++;
	}
	return nullptr;
}

//done
User * TriviaServer::getUserBySocket(tcp::socket* socket)
{
	std::map<tcp::socket*, User*>::iterator it = _connectedUsers.find(socket);
	if (it != _connectedUsers.end())
	{
		return(it->second);
	}
	return(nullptr);
}
//done
Room * TriviaServer::getRoomById(int id)
{
	std::map<int, Room*>::iterator it = _roomList.find(id);
	if (it != _roomList.end())
	{
		return(it->second);
	}
	return(nullptr);
}
