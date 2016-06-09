#include "Game.h"

Game::Game(std::vector<std::shared_ptr<User>> players, int questionsNo, std::shared_ptr<DB> db):_users(players),_db(db)
{
	_questionsNo = questionsNo;
}

bool Game::leaveGame(std::shared_ptr<User> user)
{
	for (std::vector<std::shared_ptr<User>>::iterator i = _users.begin(); i != _users.end(); i++)
	{
		user->_username;
		if ((*i)->_username == user->_username)
		{
			_users.erase(i);
			user->send(std::to_string(LEAVE_ROOM_REPLY_SUCCESS));
			//this->sendMessage(this->getUsersListMessage());
			//;
		}
	}
	return true;
}
