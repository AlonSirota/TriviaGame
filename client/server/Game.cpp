#include "Game.h"

Game::Game(std::vector<std::shared_ptr<User>> players, int questionsNo, std::shared_ptr<DB> db):_users(players),_db(db)
{
	_questionsNo = questionsNo;
	_id = _db->insertNewGame();//need to check if failed

	_questions = _db->questionInit(questionsNo); //this may throw. should catch in whoever created the Game.

	//this code will only be invoked if no exception was thrown.
	for (std::vector<std::shared_ptr<User>>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		(*it)->_currGameID = _id;
		_results.insert(std::pair<std::string, int>((*it)->getUsername(), 0));
	}
}

void Game::handleFinishGame()
{
	_db->updateGameStatus(_id);
	//send message 121
	std::string message = std::to_string(END_GAME);
	message += std::to_string(_users.size());
	for (int i = 0; i < _users.size(); i++)
	{
		message += Helper::getPaddedNumber((_users[i])->getUsername().length(), 2);
		message += (_users[i])->getUsername();
		message += Helper::getPaddedNumber(_results[(_users[i])->getUsername()], 2);
		(_users[i])->_currGameID = 0;
	}
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

bool Game::handleNextTurn()
{
	_currQuestionIndex++;
	if (_users.size() == 0)
	{
		handleFinishGame();
		return false;
	}
	if (_users.size() == _currentTurnAnswers)
	{
		if (_questionsNo == _currQuestionIndex)
		{
			handleFinishGame();
		}
		else
		{
			//_currQuestionIndex++;
			sendQuestionToAllUsers();
		}
	}
	return true;
}

bool Game::handleAnswerFromUser(std::shared_ptr<User> user, int answerNo, int time)
{
	_currentTurnAnswers++;
	bool isCorrect = false;
	if (answerNo - 1 == _questions[_currQuestionIndex]->getCorrectAnswerIndex()) //the -1 is there because answerNo index starts at 1, but the index of correct answer starts at 0.
	{
		_results[user->_username]++;
		isCorrect = true;
	}
	
	if (answerNo == 5)
	{
		_db->addAnswerToUser(_id,
			user->_username,
			_questions[_currQuestionIndex]->getId(),
			"",
			isCorrect,
			time);
	}
	else
	{
		_db->addAnswerToUser(_id,
			user->_username,
			_questions[_currQuestionIndex]->getId(),
			_questions[_currQuestionIndex]->getAnswers()[answerNo],
			isCorrect,
			time);
	}
	
	if (isCorrect)
	{
		user->send(std::to_string(CLIENT_ANSWER_REPLY) + "1");
	}
	else
	{
		user->send(std::to_string(CLIENT_ANSWER_REPLY) + "0");
	}
	return handleNextTurn();
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
	message += Helper::getPaddedNumber((_questions[_currQuestionIndex])->getQuestion().length(), 3);
	message += (_questions[_currQuestionIndex])->getQuestion();
	for (int i = 0; i < _questions[_currQuestionIndex]->getAnswers().size(); i++)
	{
		message += Helper::getPaddedNumber((_questions[_currQuestionIndex])->getAnswers()[i].length(), 3);
		message += (_questions[_currQuestionIndex])->getAnswers()[i];
	}
	_currentTurnAnswers = 0;
	for (std::vector<std::shared_ptr<User>>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		try
		{
			(*it)->send(message);
		}
		catch (...)
		{

		}
	}
}
