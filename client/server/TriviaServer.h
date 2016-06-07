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
#include <boost\bind.hpp>

using boost::asio::ip::tcp;

class TriviaServer
{
public:
	TriviaServer();//done
	void serve();//done, not checked

	Room getRoomById(int);//done
	Game getGamebyId(int);

	std::map<int, Game> getGameList() { return(_gameList); }
private:
	boost::asio::io_service _io_service;
	std::map<User, std::shared_ptr<tcp::socket>> _connectedUsers;
	DB _db;
	std::map<int, Room> _roomList;
	std::map<int, Game> _gameList;
	std::mutex _mtxMessagesRecieved;
	std::condition_variable _cvMessages;
	std::queue<recievedMessage> _queRcvMessages;
	int _roomIdSequence;
	int _gameIdSequence;

	void clientHandler(std::shared_ptr<tcp::socket>);//done, not debugged

	void safeDeleteUser(recievedMessage&);//done

	bool handleSignin(recievedMessage&);//done
	bool handleSignup(recievedMessage&);//done for first stage
	void handleSignout(recievedMessage&);//done

	void handleLeaveGame(recievedMessage&);//not in first section
	void handleStartGame(recievedMessage&);//not in first section
	void handleUserAnswer(recievedMessage&);//not in first section

	bool handleCreateRoom(recievedMessage&);//TODO
	bool handleCloseRoom(recievedMessage&);//TODO
	bool handleJoinRoom(recievedMessage&);//TODO
	bool handleLeaveRoom(recievedMessage&);//TODO
	void handleGetUsersInRoom(recievedMessage&);//TODO
	void handleGetRooms(recievedMessage&);//TODO

	void handleGetBestScores(recievedMessage&);//not in first section
	void handlegetPersonalStatus(recievedMessage&);//not in first section

	void handleRecievedMessages();//in progress (alon)
	void callHandler(recievedMessage&); //alon
	void addRecievedMessage(recievedMessage&);//alon
	recievedMessage buildRecievedMessage(std::shared_ptr<tcp::socket>,int);//done

	//User getUserByName(std::string);//if needed implement
	User getUserBySocket(std::shared_ptr<tcp::socket>);//done
	bool userExists(std::string);
	int closeRoom(User&);
};