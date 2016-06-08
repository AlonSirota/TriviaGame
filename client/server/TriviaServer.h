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

	std::shared_ptr<Room> getRoomById(int);//done
	std::shared_ptr<Game> getGamebyId(int);

	std::map<int, std::shared_ptr<Game>> getGameList() { return(_gameList); }
private:
	boost::asio::io_service _io_service;
	std::map<std::shared_ptr<User>, std::shared_ptr<tcp::socket>> _connectedUsers;
	DB _db;
	std::map<int, std::shared_ptr<Room>> _roomList;
	std::map<int, std::shared_ptr<Game>> _gameList;
	std::mutex _mtxMessagesRecieved;
	std::condition_variable _cvMessages;
	std::queue<recievedMessage> _queRcvMessages;
	int _roomIdSequence;
	int _gameIdSequence;
	int _tempUserSequence;

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
	std::shared_ptr<User> getUserBySocket(std::shared_ptr<tcp::socket>);//done
	bool userExists(std::string);
	int closeRoom(std::shared_ptr<User>);
};