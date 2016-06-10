#include "Game.h"

Game::Game(std::vector<std::shared_ptr<User>> players, int questionsNo, std::shared_ptr<DB> db):_users(players),_db(db)
{
	_questionsNo = questionsNo;
}

bool Game::handleNextTurn()
{
	return false;
}

bool Game::leaveGame(std::shared_ptr<User> user)
{
	for (std::vector<std::shared_ptr<User>>::iterator i = _users.begin(); i != _users.end(); i++)
	{
		user->_username;
		if ((*i)->_username == user->_username)
		{
			_users.erase(i);
			return handleNextTurn();
		}
	}
	return true;
}

void Game::sendQuestionToAllUsers()
{
	std::string message = std::to_string(SEND_QUESTION);
	for (int i = 0; i < _questions.size(); i++)
	{
		message += Helper::getPaddedNumber((_questions[i])->getQuestion().length(), 3);
		message += (_questions[i])->getQuestion();
	}
	_currentTurnAnswer = 0;
	std::vector<std::shared_ptr<User>>::iterator it = _users.begin();
	while (it != _users.end())
	{
		try
		{
			(*it)->send(message);
		}
		catch (...)
		{

		}
		it++;
	}
}
