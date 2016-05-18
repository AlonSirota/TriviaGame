#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
using boost::asio::ip::tcp;

class User
{
	User(std::string, tcp::socket);



private:
	std::string _username;
	Room* currRoom;
	Game* currGame;
	tcp::socket _socket;
};