#pragma once

#include <queue>
#include <mutex>

class TriviaServer
{
public:
	TriviaServer();
	~TriviaServer();
	void Server();

private:
	std::map<tcp::socket*, User*> _connectedUsers;
	tcp::socket* _socket;
	DB _db;
	std::map<int, Room*> _roomList;
	std::mutex _mtxMessagesRecieved;
	std::queue<recievedMessage*> queRcvMessages;
	static int _roomIdSequence;

	void bindAndListen();
	void accept();
	void clientHandler(tcp::socket);
	void safeDeleteUser(recievedMessage*);//not in first section

	User* handleSignin(recievedMessage*);
	bool handleSignup(recievedMessage*);
	void handleSignout(recievedMessage*);

	void handleLeaveGame(recievedMessage*);//not in first section
	void handleStartGame(recievedMessage*);//not in first section
	void handleUserAnswer(recievedMessage*);//not in first section

	bool handleCreateRoom(recievedMessage*);
	bool handleCloseRoom(recievedMessage*);
	bool handleJoinRoom(recievedMessage*);
	bool handleLeaveRoom(recievedMessage*);
	void handleGetUsersInRoom(recievedMessage*);
	void handleGetRooms(recievedMessage*);

	void handleGetBestScores(recievedMessage*);//not in first section
	void handlegetPersonalStatus(recievedMessage*);//not in first section

	void handleRecievedMessages(recievedMessage*);
	void addRecievedMessage(recievedMessage*);
	recievedMessage* buildRecievedMessage(tcp::socket,int);

	User* getUserByName(std::string);
	User* getUserBySocket(tcp::socket);
	Room* getRoomById(int);
};