#pragma once

#include <queue>
#include <mutex>
#include <map>
#include <boost\asio.hpp>
#include "DB.h"
#include "User.h"
#include "Room.h"
#include "RecievdMessage.h"
#include "Validator.h"
using boost::asio::ip::tcp;

class TriviaServer
{
public:
	TriviaServer();//done
	~TriviaServer();//done
	void Server();//alon

private:
	boost::asio::io_service _io_service;
	std::map<tcp::socket*, User*> _connectedUsers;
	tcp::socket* _socket;
	DB _db;
	std::map<int, Room*> _roomList;
	std::mutex _mtxMessagesRecieved;
	std::queue<recievedMessage*> queRcvMessages;
	//static int _roomIdSequence; - there is a link error with the word static - do we really need it to be static?
	int _roomIdSequence;

	void bindAndListen();//alon
	void accept();//alon
	void clientHandler(tcp::socket);//alon
	void safeDeleteUser(recievedMessage*);

	User* handleSignin(recievedMessage*);//done
	bool handleSignup(recievedMessage*);//done for first stage
	void handleSignout(recievedMessage*);//done

	void handleLeaveGame(recievedMessage*);//not in first section
	void handleStartGame(recievedMessage*);//not in first section
	void handleUserAnswer(recievedMessage*);//not in first section

	bool handleCreateRoom(recievedMessage*);//done
	bool handleCloseRoom(recievedMessage*);//done - CHECK IF NEED TO SEND NOTICE TO CLIENT
	bool handleJoinRoom(recievedMessage*);//done
	bool handleLeaveRoom(recievedMessage*);//NOT FINISHED YET
	void handleGetUsersInRoom(recievedMessage*);
	void handleGetRooms(recievedMessage*);

	void handleGetBestScores(recievedMessage*);//not in first section
	void handlegetPersonalStatus(recievedMessage*);//not in first section

	void handleRecievedMessages(recievedMessage*);//alon
	void addRecievedMessage(recievedMessage*);//alon
	recievedMessage* buildRecievedMessage(tcp::socket*,int);//done

	User* getUserByName(std::string);//done
	User* getUserBySocket(tcp::socket*);//done
	Room* getRoomById(int);//done
};