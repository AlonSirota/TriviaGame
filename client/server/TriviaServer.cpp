#include "TriviaServer.h"

TriviaServer::TriviaServer()//:_db() - only in later version
{
	_socket = new tcp::socket(_io_service);
}

TriviaServer::~TriviaServer()
{
	delete _socket;
}












recievedMessage * TriviaServer::buildRecievedMessage(tcp::socket* socket, int messCode)
{
	std::string code = std::to_string(messCode);
	switch (messCode)
	{

	}
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
//done
void TriviaServer::handleSignout(recievedMessage* message)
{
	User* user = message->getUser();
	if (user != nullptr)
	{
		handleCloseRoom(message);
		handleLeaveRoom(message);
		//handleLeaveGame - only in later version
		_connectedUsers.erase(_connectedUsers.find(message->getSocket()));
	}
}
//done
bool TriviaServer::handleCreateRoom(recievedMessage* message)
{
	User* user = message->getUser();
	if (user == nullptr)
	{
		return(false);
	}
	_roomIdSequence++;
	bool ans = user->createRoom(_roomIdSequence, message->getValues()[0], atoi(message->getValues()[1].c_str()), atoi(message->getValues()[2].c_str()), atoi(message->getValues()[3].c_str()));
	if (ans)
	{
		_roomList.insert(std::pair<int, Room*>(_roomIdSequence, user->getRoom()));
	}
	return ans;
}
//done - CHECK IF NEED TO SEND NOTICE TO CLIENT
bool TriviaServer::handleCloseRoom(recievedMessage* message)
{
	User* user = message->getUser();
	if (user->getRoom() == nullptr)
	{
		return(false);
	}
	int ans = user->closeRoom();
	if (ans == -1)
	{
		return(false);
	}
	_roomList.erase(ans);
	return(true);
}

bool TriviaServer::handleJoinRoom(recievedMessage* message)
{
	User* user = message->getUser();
	if (user == nullptr)
	{
		return(false);
	}
	int roomId = atoi(message->getValues()[0].c_str());
	Room* room = getRoomById(roomId);
	if (room == nullptr)
	{
		//send failed message to user code 1102
	}
	bool ans = user->joinRoom(room); //message if failed or succeeded is sent in Room::joinRoom
	return ans;
}
//not done yet
bool TriviaServer::handleLeaveRoom(recievedMessage* message)
{
	User* user = message->getUser();
	if (user == nullptr)
	{
		return(false);
	}
	Room* room = user->getRoom();
	if (room == nullptr)
	{
		return(false);
	}
	user->leaveRoom();
	return true;
}
