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
#include "Game.h"
#include <boost\bind.hpp>

using boost::asio::ip::tcp;

class TriviaServer
{
public:
	TriviaServer();
	void serve();

	std::shared_ptr<Room> getRoomById(int);
	std::shared_ptr<Game> getGamebyId(int);

	std::map<int, std::shared_ptr<Game>> getGameList() { return(_gameList); }
private:
	boost::asio::io_service _io_service;
	std::map<std::shared_ptr<User>, std::shared_ptr<tcp::socket>> _connectedUsers;
	std::shared_ptr<DB> _db; //TODO make this a non-pointer
	std::map<int, std::shared_ptr<Room>> _roomList;
	std::map<int, std::shared_ptr<Game>> _gameList;
	std::mutex _mtxMessagesRecieved;
	std::condition_variable _cvMessages;
	std::queue<recievedMessage> _queRcvMessages;
	int _roomIdSequence;
	int _tempUserSequence;

	void clientHandler(std::shared_ptr<tcp::socket>);

	void safeDeleteUser(recievedMessage&);

	bool handleSignin(recievedMessage&); //DEBUGGED
	bool handleSignup(recievedMessage&);
	void handleSignout(recievedMessage&);

	void handleLeaveGame(recievedMessage&);//not in first section
	void handleStartGame(recievedMessage&);//not in first section
	void handleUserAnswer(recievedMessage&);//not in first section

	bool handleCreateRoom(recievedMessage&);
	bool handleCloseRoom(recievedMessage & message, bool startGame);
	bool handleJoinRoom(recievedMessage&);
	void handleLeaveRoom(recievedMessage&);
	void handleGetUsersInRoom(recievedMessage&);
	void handleGetRooms(recievedMessage&);

	void handleGetBestScores(recievedMessage&);//not in first section
	void handlegetPersonalStatus(recievedMessage&);//not in first section

	void handleRecievedMessages();
	void callHandler(recievedMessage&);
	void addRecievedMessage(recievedMessage&);
	recievedMessage buildRecievedMessage(std::shared_ptr<tcp::socket>,int);

	std::shared_ptr<User> getUserBySocket(std::shared_ptr<tcp::socket>);
	bool userExists(std::string);
	int closeRoom(std::shared_ptr<User> user, bool startGame);
	bool TriviaServer::isUserAdminOfRoom(const std::shared_ptr<User> &user);
};