#pragma once

#include <queue>
#include <mutex>
#include <map>
#include <boost\asio.hpp>
//#include <boost/lockfree/queue.hpp>
#include "DB.h"
#include "User.h"
#include "Room.h"
#include "RecievdMessage.h"
#include "Validator.h"
#include <boost\bind.hpp>
//#include <boost\thread.hpp>
//#include <boost\lockfree\queue.hpp>
using boost::asio::ip::tcp;

class TriviaServer
{
public:
	TriviaServer();//done
	void serve();//done, not checked

	Room& getRoomById(int);//done
	Game& getGamebyId(int);

	std::map<int, Game> getGameList() { return(_gameList); }
private:
	boost::asio::io_service _io_service;
	std::map<User, tcp::socket> _connectedUsers;
	//tcp::socket _socket;
	DB _db;
	std::map<int, Room> _roomList;
	std::map<int, Game> _gameList;
	std::mutex _mtxMessagesRecieved;
	std::unique_lock<std::mutex> _ulMessagesReceived;
	std::condition_variable _cvMessages;
	//boost::lockfree::queue<recievedMessage> _queRcvMessages;
	std::queue<recievedMessage> _queRcvMessages;
	int _roomIdSequence;
	int _gameIdSequence;
	//to save actual objects
	std::vector<User> _users;
	std::vector<Room> _rooms;
	std::vector<Game> _games;

	void clientHandler(tcp::socket&);//done, not debugged

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
	recievedMessage buildRecievedMessage(tcp::socket&,int);//done

	User& getUserByName(std::string);//done
	User& getUserBySocket(tcp::socket&);//done
	bool userExists(std::string);
};