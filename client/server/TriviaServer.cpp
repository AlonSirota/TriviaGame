#include "TriviaServer.h"
//done
TriviaServer::TriviaServer() : _cvMessages()
{
	std::thread handleRecievedMessagesThread(&TriviaServer::handleRecievedMessages, this);
	handleRecievedMessagesThread.detach();
	_roomIdSequence = 1;
	_gameIdSequence = 1;
	_tempUserSequence = 1;
	_db = std::make_shared<DB>();
}

void TriviaServer::serve()
{
	tcp::endpoint ep(tcp::v4(), 8820);
	tcp::acceptor acceptor(_io_service, ep);

	while (true)
	{
		std::cout << "Listening..." << std::endl;
		tcp::socket newSocket(_io_service);
		boost::system::error_code ec;
		acceptor.accept(newSocket, ep, ec);
		if (ec)
		{
			std::cout << "accept failed: " << ec.value();
		}
		else
		{
			std::cout << "accepted connection\n";
			std::shared_ptr<tcp::socket> ptr = std::make_shared<tcp::socket>(std::move(newSocket));
			std::thread t(&TriviaServer::clientHandler, this, ptr);
			t.detach();
		}
	}
}

void TriviaServer::handleGetBestScores(recievedMessage& message)
{
	std::cout << "handleGetBestScores was called but is yet to be implemented.\n";
	std::vector<std::pair<std::string,std::string>> ansVector = _db->getBestScores(3);
	std::string sendMessage = "124";
	for (int i = 0; i < ansVector.size(); i++)
	{
		sendMessage += Helper::getPaddedNumber(ansVector[i].first.length(), 2);
		sendMessage += ansVector[i].first;
		sendMessage += Helper::getPaddedNumber(std::atoi(ansVector[i].second.c_str()), 6);
	}
	message._user->send(sendMessage);
}

void TriviaServer::handlegetPersonalStatus(recievedMessage& message)//debugged
{
	std::vector<std::string> ansVector = _db->getPersonalStatus(message._user->getUsername());
	std::string sendMessage = "126";
	sendMessage += Helper::getPaddedNumber(std::atoi(ansVector[0].c_str()), 4);
	sendMessage += Helper::getPaddedNumber(std::atoi(ansVector[1].c_str()), 6);
	sendMessage += Helper::getPaddedNumber(std::atoi(ansVector[2].c_str()), 6);
	std::string::size_type pos = ansVector[3].find('.');
	std::string temp = ansVector[3].substr(pos+1, 2);
	sendMessage += Helper::getPaddedNumber(std::atoi(ansVector[3].substr(0, pos).c_str()),2);
	sendMessage += Helper::getPaddedNumber(std::atoi(temp.c_str()), 2);
	message._user->send(sendMessage);
}

void TriviaServer::handleRecievedMessages()
{
	std::unique_lock<std::mutex> _ulMessagesReceived(_mtxMessagesRecieved);
	while (true)
	{
		if (_queRcvMessages.empty())
		{
			_cvMessages.wait(_ulMessagesReceived); //waits for a message to be entered.
		}
		recievedMessage msg = _queRcvMessages.front();
		_queRcvMessages.pop();

		std::cout << msg.toString();
		
		callHandler(msg);
	}
}

void TriviaServer::callHandler(recievedMessage &msg) //next function to debug
{
	switch (msg._messageCode)
	{
	case SIGNIN_REQUEST: //debugged
		handleSignin(msg);
		break;
	case SIGNOUT_REQUEST: // HAS BUG
		handleSignout(msg);
		break;
	case SIGNUP_REQUEST: //debugged
		handleSignup(msg);
		break;
	case EXISTING_ROOM_REQUEST: //debugged
		handleGetRooms(msg);
		break;
	case JOIN_ROOM_REQUEST: //debugged
		handleJoinRoom(msg);
		break;
	case USERS_IN_ROOM_REQUEST: //debugged
		handleGetUsersInRoom(msg);
		break;
	case LEAVE_ROOM_REQUEST: //debugged
		handleLeaveRoom(msg);
		break;
	case CREATE_ROOM_REQUEST: //debugged
		handleCreateRoom(msg);
		break;
	case CLOSE_ROOM_REQUEST: //debugged
		handleCloseRoom(msg);
		break;
	case START_GAME_REQUEST: //debugged
		handleStartGame(msg);
		break;
	case LEAVE_GAME_REQUEST:
		handleLeaveGame(msg);
		break;
	case BEST_SCORE_REQUEST: //debugged
		handleGetBestScores(msg);
		break;
	case PERSONAL_STATE_REQUEST: //debugged
		handlegetPersonalStatus(msg);
		break;
	case CLIENT_ANSWER:
		handleUserAnswer(msg);
		break;
	default:
		std::cout << "callHandler recieved an unknown message number: " << msg._messageCode << "\n";
		//case 299 and 0 is already handled in clientHandler()
	}
}

void TriviaServer::addRecievedMessage(recievedMessage& message)
{
	_mtxMessagesRecieved.lock();
	_queRcvMessages.push(message);
	_mtxMessagesRecieved.unlock();
	_cvMessages.notify_one();
}

//done
recievedMessage TriviaServer::buildRecievedMessage(std::shared_ptr<tcp::socket> socket, int messCode)
{
	std::vector<std::string> info;
	switch (messCode) //parses the string to the string vector _values.
	{
	case SIGNIN_REQUEST:
	{
		int usernameLength = Helper::getIntPartFromSocket(socket, 2);
		info.push_back(Helper::getPartFromSocket(socket, usernameLength).data());
		int passLength = Helper::getIntPartFromSocket(socket, 2);
		info.push_back(Helper::getPartFromSocket(socket, passLength).data());
		break;
	}
	case SIGNUP_REQUEST:
	{
		int usernameLength = Helper::getIntPartFromSocket(socket, 2);
		info.push_back(Helper::getPartFromSocket(socket, usernameLength).data());
		int passLength = Helper::getIntPartFromSocket(socket, 2);
		info.push_back(Helper::getPartFromSocket(socket, passLength).data());
		int emailLength = Helper::getIntPartFromSocket(socket, 2);
		info.push_back(Helper::getPartFromSocket(socket, emailLength).data());
		break;
	}
	case USERS_IN_ROOM_REQUEST:
	case JOIN_ROOM_REQUEST:
	{
		info.push_back(Helper::getPartFromSocket(socket, 4).data());
		break;
	}
	case CREATE_ROOM_REQUEST:
	{
		int rommNameLength = Helper::getIntPartFromSocket(socket, 2);
		info.push_back(Helper::getPartFromSocket(socket, rommNameLength).data());
		info.push_back(Helper::getPartFromSocket(socket, 1).data());
		info.push_back(Helper::getPartFromSocket(socket, 2).data());
		info.push_back(Helper::getPartFromSocket(socket, 2).data());
		break;
	}
	case START_GAME_REQUEST://not in current version
		break;
	case CLIENT_ANSWER:
	{
		info.push_back(Helper::getPartFromSocket(socket, 1).data()); //answer index
		info.push_back(Helper::getPartFromSocket(socket, 2).data()); //answer time
		break;
	}		
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

	return recievedMessage(socket, messCode, info, getUserBySocket(socket));

	/*
	if (info.empty())
	{
		return(recievedMessage(socket, messCode, getUserBySocket(socket)));
	}
	else
	{
		
		if (messCode == SIGNIN_REQUEST)
		{
			//signin
			if (!userExists(info[0]))
			{
				_connectedUsers.insert(std::pair<std::shared_ptr<User>, std::shared_ptr<tcp::socket>>(std::make_shared<User>(User(info[0], socket)), socket));
				Helper::sendData(socket, std::to_string(SIGNIN_REPLY) + std::to_string(0));
			}
			else
			{
				Helper::sendData(socket, std::to_string(SIGNIN_REPLY) + std::to_string(2));
			}
			return(recievedMessage(socket, messCode, getUserBySocket(socket)));
		}
		else
		{
			return(recievedMessage(socket, messCode, info, getUserBySocket(socket)));
		}
	}*/
}

//done
std::shared_ptr<User> TriviaServer::getUserBySocket(std::shared_ptr<tcp::socket> socket)
{
	std::map<std::shared_ptr<User>, std::shared_ptr<tcp::socket>>::iterator it = _connectedUsers.begin();
	while (it != _connectedUsers.end())
	{
		if (it->second == socket)
			return(it->first);
		it++;
	}
	//return it->first; is this supposed to return the last one? because this line always accesses bad memory and crashes.
	return nullptr;
}

bool TriviaServer::userExists(std::string username) //TODO fix this according to the flipped map.
{
	std::map<std::shared_ptr<User>, std::shared_ptr<tcp::socket>>::iterator it = _connectedUsers.begin();
	while (it != _connectedUsers.end())
	{
		if (it->first->getUsername() == username)
			return(true);
		it++;
	}
	return false;
}

int TriviaServer::closeRoom(std::shared_ptr<User>  user)
{
	bool found = false;
	if (_roomList.count(user->_currRoomID)) //if room exists
	{
		std::shared_ptr<Room> room = getRoomById(user->_currRoomID);
		int ret = room->closeRoom(user);

		if (ret == -1) //if it failed (user isn't admin)
		{
			return -1;
		}
		else
		{
			_roomList.erase(ret);
			return ret;
		}
	}
	else
	{
		return -1;
	}
}

//done
std::shared_ptr<Room> TriviaServer::getRoomById(int id)
{
	std::map<int, std::shared_ptr<Room>>::iterator it = _roomList.find(id);
	if (it != _roomList.end())
	{
		return(it->second);
	}
	return(it->second);
}

std::shared_ptr<Game> TriviaServer::getGamebyId(int id)
{
	std::map<int, std::shared_ptr<Game>>::iterator it = _gameList.find(id);
	if (it != _gameList.end())
	{
		return(it->second);
	}
	return(_gameList.end()->second);
}

void TriviaServer::clientHandler(std::shared_ptr<tcp::socket> s)
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
		std::shared_ptr<tcp::socket> socket = message._socket;
		handleSignout(message);
		socket->close();
	}
	catch (const std::exception& ex)
	{
		std::cout << "exeption caught in safeDeleteUser:" << ex.what() << std::endl;
	}
}
//done
bool TriviaServer::handleSignin(recievedMessage& message)//debugged
{
	if(_db->isUserAndPassMatch(message._values[0], message._values[1]))
	{
		if (!userExists(message._values[0]))
		{
			//success connecting
			_connectedUsers.insert(std::pair<std::shared_ptr<User>, std::shared_ptr<tcp::socket>>(std::make_shared<User>(User(message._values[0], message._socket)), message._socket));
			Helper::sendData(message._socket, std::to_string(SIGNIN_REPLY) + std::to_string(0)); //success
			return(true);
		}
		else
		{
			Helper::sendData(message._socket, std::to_string(SIGNIN_REPLY) + std::to_string(2)); //user alread connected
			return false;
		}
	}
	else
	{
		Helper::sendData(message._socket, std::to_string(SIGNIN_REPLY) + std::to_string(1)); //wrong detailes
		return false;
	}
}
//done for first stage

bool TriviaServer::handleSignup(recievedMessage& message)
{
	if (!Validator::isUsernameValid(message._values[0]))
	{
		//fail - username not valid	
		Helper::sendData(std::move(message._socket), std::to_string(SIGNUP_REPLY) + std::to_string(3));
		return(false);
	}
	else if (!Validator::isPasswordValid(message._values[1]))
	{
		//fail - password not valid
		Helper::sendData(std::move(message._socket), std::to_string(SIGNUP_REPLY) + std::to_string(1));
		return(false);
	}
	else if (userExists(message._values[0]))
	{
		//fail - user already exists
		Helper::sendData(std::move(message._socket), std::to_string(SIGNUP_REPLY) + std::to_string(2));
		return(false);
	}
	//success
	_db->addNewUser(message._values[0], message._values[1], message._values[2]);
	Helper::sendData(std::move(message._socket), std::to_string(SIGNUP_REPLY) + std::to_string(0));
	return(true);
}
//done
void TriviaServer::handleSignout(recievedMessage& message)
{
	//std::shared_ptr<User> user = message._user;
	//handleCloseRoom(message);
	handleLeaveRoom(message);
	//handleLeaveGame - only in later version
	_connectedUsers.erase(_connectedUsers.find(message._user));
}

void TriviaServer::handleLeaveGame(recievedMessage &msg) // not debugged
{
	//std::cout << "handleLeaveGame was called but isn't implemented yet\n";
	_gameList[msg._user->_currGameID]->leaveGame(msg._user);
	handleLeaveRoom(msg);
	msg._user->_currGameID = 0;
}

void TriviaServer::handleStartGame(recievedMessage &msg)//debugged
{
	//std::cout << "handleStartGame was called but isn't implemented yet\n";
	Game currentGame(_roomList[msg._user->_currRoomID]->_users, _roomList[msg._user->_currRoomID]->_questionsNo, _db);
	_gameList.insert(std::pair<int, std::shared_ptr<Game>>(currentGame._id, std::make_shared<Game>(currentGame)));
	currentGame.sendQuestionToAllUsers();
	handleCloseRoom(msg);
}

void TriviaServer::handleUserAnswer(recievedMessage &msg)
{
	//std::cout << "handleUserAnswer was called but isn't implemented yet\n";
	std::shared_ptr<Game> game = _gameList[msg._user->_currGameID];

	if (game != nullptr)
	{
		if (!game->handleAnswerFromUser(msg._user, stoi(msg._values[0]), stoi(msg._values[1])))
		{ //means the game has ended.
			_gameList.erase(game->_id);
		}
	}
}
//done
bool TriviaServer::handleCreateRoom(recievedMessage& message) // check this
{
	std::shared_ptr<User> user = message._user;
	if (!_roomList.count(user->_currRoomID))
	{
		int roomIdTemp = _roomIdSequence++;
		Room currentRoom(roomIdTemp, user, message._values[0], atoi(message._values[1].c_str()), atoi(message._values[2].c_str()), atoi(message._values[3].c_str()));
		_roomList.insert(std::pair<int, std::shared_ptr<Room>>(roomIdTemp, std::make_shared<Room>(currentRoom)));
		user->_currRoomID = roomIdTemp;
		Helper::sendData(message._socket,std::to_string(CREATE_ROOM_SUCSESS));
		currentRoom.sendMessage(currentRoom.getUsersListMessage());
		return(true);
	}
	else
	{
		Helper::sendData(message._socket, std::to_string(CREATE_ROOM_FAILED));
		return false;
	}
}

//done - CHECK IF NEED TO SEND NOTICE TO CLIENT
bool TriviaServer::handleCloseRoom(recievedMessage& message)
{
	std::shared_ptr<User> user = message._user;
	if (_roomList.count(user->_currRoomID))
	{
		int ans = this->closeRoom(user);
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
		Helper::sendData(std::move(message._socket), std::to_string(JOIN_ROOM_REPLY) + std::to_string(2));
	}
	std::shared_ptr<Room> room = getRoomById(roomId);
	bool ans = message._user->joinRoom(roomId);
	room->joinRoom(message._user);//message if failed or succeeded is sent in Room::joinRoom
	return ans;
}
//done
bool TriviaServer::handleLeaveRoom(recievedMessage& message)
{
	std::shared_ptr<User> user = message._user;
	if (_roomList.count(user->_currRoomID))
	{
		std::shared_ptr<Room> room = getRoomById(user->_currRoomID);
		user->leaveRoom();
		room->leaveRoom(user);
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
	int roomId = stoi(message._values[0]);
	if (_roomList.count(roomId))
	{
		std::shared_ptr<Room> room = getRoomById(roomId);
		std::string sendString = room->getUsersListMessage();
		Helper::sendData(message._socket, sendString);
	}
	else
	{
		//fail - no room has this id
		Helper::sendData(std::move(message._socket), std::to_string(USERS_IN_ROOM_REPLY) + std::to_string(0));
	}
}
//done
void TriviaServer::handleGetRooms(recievedMessage& message)
{
	std::string sendString = std::to_string(EXISTING_ROOM_REPLY);
	sendString += Helper::getPaddedNumber(_roomList.size(), 4);
	std::map<int, std::shared_ptr<Room>>::iterator it = _roomList.begin();
	while (it != _roomList.end())
	{
		sendString += Helper::getPaddedNumber(it->second->_id, 4);
		sendString += Helper::getPaddedNumber(it->second->_name.length(), 2);
		sendString += it->second->_name;
		it++;
	}
	Helper::sendData(std::move(message._socket), sendString);
}