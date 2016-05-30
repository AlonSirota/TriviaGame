#pragma once

#include <queue>
#include <mutex>
#include <map>
#include <boost\asio.hpp>
#include <boost/lockfree/queue.hpp>
#include "DB.h"
#include "User.h"
#include "Room.h"
#include "RecievdMessage.h"
#include "Validator.h"
#include <boost\bind.hpp>
#include <boost\thread.hpp>
using boost::asio::ip::tcp;

class TriviaServer
{
public:
	TriviaServer();//done
	void serve();//alon

	Room& getRoomById(int);//done
	Game& getGamebyId(int);

	std::map<int, Game&> getGameList() { return(_gameList); }
private:
	boost::asio::io_service _io_service;
	std::map<tcp::socket&, User&> _connectedUsers;
	tcp::socket _socket;
	DB _db;
	std::map<int, Room&> _roomList;
	std::map<int, Game&> _gameList;
	std::mutex _mtxMessagesRecieved;
	std::queue<recievedMessage> queRcvMessages;
	int _roomIdSequence;
	int _gameIdSequence;

	void clientHandler(tcp::socket&);//done, not debugged
	void safeDeleteUser(recievedMessage&);//done
	void safeDeleteUser(recievedMessage&);//done

	User& handleSignin(recievedMessage&);//done
	bool handleSignup(recievedMessage&);//done for first stage
	void handleSignout(recievedMessage&);//done

	void handleLeaveGame(recievedMessage&);//not in first section
	void handleStartGame(recievedMessage&);//not in first section
	void handleUserAnswer(recievedMessage&);//not in first section

	bool handleCreateRoom(recievedMessage&);//done
	bool handleCloseRoom(recievedMessage&);//done - CHECK IF NEED TO SEND NOTICE TO CLIENT
	bool handleJoinRoom(recievedMessage&);//done
	bool handleLeaveRoom(recievedMessage&);//done
	void handleGetUsersInRoom(recievedMessage&);//done
	void handleGetRooms(recievedMessage&);//done

	void handleGetBestScores(recievedMessage&);//not in first section
	void handlegetPersonalStatus(recievedMessage&);//not in first section

	void handleRecievedMessages(recievedMessage&);//alon
	void addRecievedMessage(recievedMessage&);//alon
	recievedMessage buildRecievedMessage(tcp::socket&,int);//done

	User& getUserByName(std::string);//done
	User& getUserBySocket(tcp::socket&);//done
};