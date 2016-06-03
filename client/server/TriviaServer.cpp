#include "TriviaServer.h"
//done
TriviaServer::TriviaServer(): _cvMessages(), _ulMessagesReceived(_mtxMessagesRecieved)//:_db() - only in later version
{
	std::thread handleRecievedMessagesThread(&TriviaServer::handleRecievedMessages, this);
	handleRecievedMessagesThread.detach();
}

void TriviaServer::serve()
{
	tcp::endpoint ep(tcp::v4(), 8820);
	tcp::acceptor acceptor(_io_service, ep);

	while (true)
	{
		tcp::socket newSocket(_io_service);
		acceptor.async_accept(newSocket, [this, &newSocket](const boost::system::error_code &ec) 
		{
			if (ec)
			{
				std::cout << "async_accept failed: " << ec.value();
			}
			else
			{
				std::thread t(&TriviaServer::clientHandler, this, newSocket); //do we need std::move here?
				t.detach();
			}
		});
	}
}

void TriviaServer::handleRecievedMessages()
{
	while (true)
	{
		if (_queRcvMessages.empty())
		{
			_cvMessages.wait(_ulMessagesReceived); //waits for a message to be entered.
		}
		recievedMessage& temp = _queRcvMessages.front();
		_queRcvMessages.pop();

		recievedMessage msg(temp._socket, temp._messageCode, getUserBySocket(temp._socket)); //An error may occur here - it's because we access a variable that was destroyed (from the queue).
		callHandler(msg);
	}	
}

void TriviaServer::callHandler(recievedMessage &msg)
{
	switch (msg._messageCode)
	{
		case SIGNIN_REQUEST:
			handleSignin(msg);
			break;
		case SIGNOUT_REQUEST:
			handleSignout(msg);
			break;
		case SIGNUP_REQUEST:
			handleSignup(msg);
			break;
		//case EXISTING_ROOM_REQUEST: handler isn't written yet TODO
		case JOIN_ROOM_REQUEST:
			handleJoinRoom(msg);
			break;
		//case USERS_IN_ROOM_REQUEST: handler isn't written yet TODO
		case LEAVE_ROOM_REQUEST:
			handleLeaveRoom(msg);
			break;
		case CREATE_ROOM_REQUEST:
			handleCreateRoom(msg);
			break;
		case CLOSE_ROOM_REQUEST:
			handleCloseRoom(msg);
			break;
		case START_GAME_REQUEST:
			handleStartGame(msg);
			break;
		//case CLIENT_ANSWER: handler isn't written yet TODO (i probably just don't know what is the name of the correct handler)
		case LEAVE_GAME_REQUEST:
			handleLeaveGame(msg);
			break;
		case BEST_SCORE_REQUEST:
			handleGetBestScores(msg);
			break;
		case PERSONAL_STATE_REQUEST:
			handlegetPersonalStatus(msg);
			break;
		default:
			std::cout << "callHandler recieved an unknown message number: " << msg._messageCode << "\n";
			//case 299 and 0 is already handled in clientHandler()
	}
}

//done
recievedMessage TriviaServer::buildRecievedMessage(tcp::socket& socket, int messCode)
{
	std::vector<std::string> info;
	switch (messCode)
	{
		case SIGNIN_REQUEST:
		{
			int usernameLength = Helper::getIntPartFromSocket(socket, 2);
			info.push_back(Helper::getPartFromSocket(socket, usernameLength));
			int passLength = Helper::getIntPartFromSocket(socket, 2);
			info.push_back(Helper::getPartFromSocket(socket, passLength));
			break;
		}
		case SIGNUP_REQUEST:
		{
			int usernameLength = Helper::getIntPartFromSocket(socket, 2);
			info.push_back(Helper::getPartFromSocket(socket, usernameLength));
			int passLength = Helper::getIntPartFromSocket(socket, 2);
			info.push_back(Helper::getPartFromSocket(socket, passLength));
			int emailLength = Helper::getIntPartFromSocket(socket, 2);
			info.push_back(Helper::getPartFromSocket(socket, emailLength));
			break;
		}
		case USERS_IN_ROOM_REQUEST:
		case JOIN_ROOM_REQUEST:
		{
			info.push_back(Helper::getPartFromSocket(socket, 4));
			break;
		}
		case CREATE_ROOM_REQUEST:
		{
			int rommNameLength = Helper::getIntPartFromSocket(socket, 2);
			info.push_back(Helper::getPartFromSocket(socket, rommNameLength));
			info.push_back(Helper::getPartFromSocket(socket, 1));
			info.push_back(Helper::getPartFromSocket(socket, 2));
			info.push_back(Helper::getPartFromSocket(socket, 2));
			break;
		}
		case START_GAME_REQUEST://not in current version
			break;
		case CLIENT_ANSWER://not in current version
			break;
		case LEAVE_GAME_REQUEST://not in current version
			break;
		case BEST_SCORE_REQUEST://not in current version
			break;
		case PERSONAL_STATE_REQUEST://not in current version
			break;
		default:
		{
		}
	}
	if (info.empty())
	{
		return(recievedMessage(socket, messCode, getUserBySocket(socket)));
	}
	else
	{
		return(recievedMessage(socket, messCode,info, getUserBySocket(socket)));
	}
}

//done
User& TriviaServer::getUserByName(std::string username)
{
	bool found = false;
	std::map<tcp::socket, User>::iterator it = _connectedUsers.begin();
	while (it != _connectedUsers.end())
	{
		if (it->second.getUsername() == username)
			return(it->second);
		it++;
	}
	return _connectedUsers.end()->second;
}

//done
User& TriviaServer::getUserBySocket(tcp::socket& socket)
{
	std::map<tcp::socket, User>::iterator it = _connectedUsers.find(socket);
	if (it != _connectedUsers.end())
	{
		return(it->second);
	}
	return(it->second);
}
bool TriviaServer::userExists(std::string username)
{
	bool found = false;
	std::map<tcp::socket, User>::iterator it = _connectedUsers.begin();
	while (it != _connectedUsers.end())
	{
		if (it->second.getUsername() == username)
			return(true);
		it++;
	}
	return false;
}
//done
Room& TriviaServer::getRoomById(int id)
{
	std::map<int, Room>::iterator it = _roomList.find(id);
	if (it != _roomList.end())
	{
		return(it->second);
	}
	return(it->second);
}

Game& TriviaServer::getGamebyId(int id)
{
	std::map<int, Game>::iterator it = _gameList.find(id);
	if (it != _gameList.end())
	{
		return(it->second);
	}
	return(_gameList.end()->second);
}

void TriviaServer::clientHandler(tcp::socket& s)
{
	int msgCode = Helper::getMessageTypeCode(s);
	while (msgCode != 0 && msgCode != EXIT)
	{
		addRecievedMessage(buildRecievedMessage(s, msgCode));
		msgCode = Helper::getMessageTypeCode(s);
	}
	addRecievedMessage(buildRecievedMessage(s, EXIT));
}

//done
void TriviaServer::safeDeleteUser(recievedMessage& message)
{
	try
	{
		tcp::socket& socket = message._socket;
		handleSignout(message);
		socket.close();
	}
	catch (const std::exception& ex)
	{
		std::cout << "exeption caught in safeDeleteUser:" << ex.what() << std::endl;
	}
}
//done
bool TriviaServer::handleSignin(recievedMessage& message)
{
	//check if user exists in database - in next part
	if (!userExists(message._user.getUsername()))
	{
		//success connecting
		_users.push_back(User(message._values[0], message._socket));
		_connectedUsers.insert(std::pair<tcp::socket, User>(message._socket, _users.back()));
		Helper::sendData(message._socket, std::to_string(SIGNIN_REPLY) + std::to_string(0));
		return(true);
	}
	else
	{
		Helper::sendData(message._socket, std::to_string(SIGNIN_REPLY) + std::to_string(2));
		return false;
	}
}
//done for first stage
bool TriviaServer::handleSignup(recievedMessage& message)
{
	if(!Validator::isUsernameValid(message._values[0]))
	{
		//fail - username not valid	
		Helper::sendData(message._socket, std::to_string(SIGNUP_REPLY) + std::to_string(3));
		return(false);
	}
	else if (!Validator::isPasswordValid(message._values[1]))
	{
		//fail - password not valid
		Helper::sendData(message._socket, std::to_string(SIGNUP_REPLY) + std::to_string(1));
		return(false);
	}
	//success
	Helper::sendData(message._socket, std::to_string(SIGNUP_REPLY) + std::to_string(0));
	return(true);
}
//done
void TriviaServer::handleSignout(recievedMessage& message)
{
	User& user = message._user;
	handleCloseRoom(message);
	handleLeaveRoom(message);
	//handleLeaveGame - only in later version
	_connectedUsers.erase(_connectedUsers.find(message._socket));
}
//done
bool TriviaServer::handleCreateRoom(recievedMessage& message)
{
	User& user = message._user;
	if (_roomList.count(user._currRoomID))
	{
		int roomIdTemp = ++_roomIdSequence;
		_rooms.push_back(Room(roomIdTemp, user,message._values[0], atoi(message._values[1].c_str()), atoi(message._values[2].c_str()), atoi(message._values[3].c_str())));
		_roomList.insert(std::pair<int, Room&>(roomIdTemp, getRoomById(user._currRoomID)));
		user.joinRoom(roomIdTemp);
		return true;
	}
	else
	{
		return false;
	}
}
//done - CHECK IF NEED TO SEND NOTICE TO CLIENT
bool TriviaServer::handleCloseRoom(recievedMessage& message)
{
	User& user = message._user;
	if (_roomList.count(user._currRoomID))
	{
		int ans = user.closeRoom();
		if (ans == -1)
		{
			return(false);
		}
		_roomList.erase(ans);
		return(true);
	}
	else
	{
		return false;
	}
}
//done
bool TriviaServer::handleJoinRoom(recievedMessage& message)
{
	int roomId = atoi(message._values[0].c_str());
	if (!_roomList.count(roomId))
	{
		//send failed message to user code 1102
		Helper::sendData(message._socket, std::to_string(JOIN_ROOM_REPLY) + std::to_string(2));
	}
	Room& room = getRoomById(roomId);
	bool ans = message._user.joinRoom(roomId); 
	room.joinRoom(message._user);//message if failed or succeeded is sent in Room::joinRoom
	return ans;
}
//done
bool TriviaServer::handleLeaveRoom(recievedMessage& message)
{
	User& user = message._user;
	if (_roomList.count(user._currRoomID))
	{
		Room& room = getRoomById(user._currRoomID);
		user.leaveRoom();
		room.leaveRoom(user);
		return true;
	}
	else
	{
		return false;
	}
}
//done
void TriviaServer::handleGetUsersInRoom(recievedMessage& message)
{
	User& user = message._user;
	if (_roomList.count(user._currRoomID))
	{
		Room& room = getRoomById(atoi(message._values[0].c_str()));
		std::string sendString = std::to_string(USERS_IN_ROOM_REPLY);
		sendString += room.getUsersListMessage();
		Helper::sendData(message._socket, sendString);
	}
	else
	{
		//fail - no room has this id
		Helper::sendData(message._socket, std::to_string(USERS_IN_ROOM_REPLY) + std::to_string(0));
	}
}
//done
void TriviaServer::handleGetRooms(recievedMessage& message)
{
	std::string sendString = std::to_string(EXISTING_ROOM_REPLY);
	sendString += Helper::getPaddedNumber(_roomList.size(), 4);
	std::map<int, Room>::iterator it = _roomList.begin();
	while (it != _roomList.end())
	{
		sendString += Helper::getPaddedNumber(it->second._id,4);
		sendString += Helper::getPaddedNumber(it->second._name.length(),2);
		sendString += it->second._name;
	}
	Helper::sendData(message._socket, sendString);
}
