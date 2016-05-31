#include "TriviaServer.h"
//done
TriviaServer::TriviaServer(): _socket(_io_service), _cvMessages(), _ulMessagesReceived(_mtxMessagesRecieved)//:_db() - only in later version
{
	boost::thread handleRecievedMessagesThread(boost::bind(&handleRecievedMessages, this));
	handleRecievedMessagesThread.detach();
}

void TriviaServer::serve()
{
	tcp::endpoint ep(tcp::v4(), 8820);
	tcp::acceptor acceptor(_io_service, ep);
	tcp::socket *newSocket;
	while (true)
	{
		newSocket = new tcp::socket(_io_service);
		acceptor.async_accept(*newSocket, [this, newSocket](const boost::system::error_code &ec) 
		{
			if (ec)
			{
				std::cout << "async_accept failed: " << ec.value();
			}
			else
			{
				boost::thread t(boost::bind(&clientHandler, this, std::move(/*&*/newSocket)));
				t.detach();
			}
		});
	}
}

void TriviaServer::handleRecievedMessages()
{
	recievedMessage msg;

	while (true)
	{
		if (_queRcvMessages.empty())
		{
			_cvMessages.wait(_ulMessagesReceived); //waits for a message to be entered.
		}
		_queRcvMessages.pop(msg);

		msg._user.copy(getUserBySocket(msg._socket));
		callHandler(msg);
	}	
}

void TriviaServer::callHandler(recievedMessage &msg)
{
	switch (msg._messageCode)
	{
		case SIGNIN_REQUEST: 
		{
			break;
		}
			
		case SIGNOUT_REQUEST:
		case SIGNUP_REQUEST:
		case EXISTING_ROOM_REQUEST:
		case JOIN_ROOM_REQUEST:
		case USERS_IN_ROOM_REQUEST:
		case LEAVE_ROOM_REQUEST:
		case CREATE_ROOM_REQUEST:
		case CLOSE_ROOM_REQUEST:
		case START_GAME_REQUEST:
		case CLIENT_ANSWER:
		case LEAVE_GAME_REQUEST:
		case BEST_SCORE_REQUEST:
		case PERSONAL_STATE_REQUEST:
		default:
			std::cout << "callHandler recieved an unknown message number: " << msg._messageCode << "\n";
			//case 299 is already handled in clientHandler()
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
	std::map<tcp::socket&, User&>::iterator it = _connectedUsers.begin();
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
	std::map<tcp::socket&, User&>::iterator it = _connectedUsers.find(socket);
	if (it != _connectedUsers.end())
	{
		return(it->second);
	}
	return(it->second);
}
//done
Room& TriviaServer::getRoomById(int id)
{
	std::map<int, Room&>::iterator it = _roomList.find(id);
	if (it != _roomList.end())
	{
		return(it->second);
	}
	return(it->second);
}

Game& TriviaServer::getGamebyId(int id)
{
	std::map<int, Game&>::iterator it = _gameList.find(id);
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
User& TriviaServer::handleSignin(recievedMessage& message)
{
	//check if user exists in database - in next part
	User& user = getUserByName(message._user.getUsername());
	if (user == nullptr)
	{
		//success connecting
		User& newUser = new User(message._values[0], message._socket);
		_connectedUsers.insert(std::pair<tcp::socket&, User&>(message._socket, newUser));
		Helper::sendData(message._socket, std::to_string(SIGNIN_REPLY) + std::to_string(0));
		return(newUser);
	}
	Helper::sendData(message._socket, std::to_string(SIGNIN_REPLY) + std::to_string(2));
	return nullptr;
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
		_roomIdSequence++;
		bool ans = user.createRoom(_roomIdSequence, message._values[0], atoi(message._values[1].c_str()), atoi(message._values[2].c_str()), atoi(message._values[3].c_str()));
		if (ans)
		{
			_roomList.insert(std::pair<int, Room&>(_roomIdSequence, getRoomById(user._currRoomID)));
		}
		return ans;
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
	bool ans = message._user.joinRoom(room); //message if failed or succeeded is sent in Room::joinRoom
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
	std::map<int, Room&>::iterator it = _roomList.begin();
	while (it != _roomList.end())
	{
		sendString += Helper::getPaddedNumber(it->second._id,4);
		sendString += Helper::getPaddedNumber(it->second._name.length(),2);
		sendString += it->second._name;
	}
	Helper::sendData(message._socket, sendString);
}
