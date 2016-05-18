#include <vector>


class Room
{
	Room(int, User*, std::string, int, int, int);
	bool joinRoom(User*);
	void leaveRoom(User*);
	int closeRoom(User*);
	std::vector<User*> getUsers();
	std::string getUsersListMessage();
	int getQuestionsNo();
	int getId();
	std::string getName();

	//add private functions and variables
};