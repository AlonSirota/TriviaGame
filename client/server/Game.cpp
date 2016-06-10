#include "Game.h"

Game::Game(std::vector<std::shared_ptr<User>> players, int questionsNo, std::shared_ptr<DB> db):_users(players),_db(db)
{
	_questionsNo = questionsNo;
	_id = _db->insertNewGame();//need to check if failed
	_questions = _db->questionInit(questionsNo);
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
		message += Helper::getPaddedNumber((_users[i])->getUsername().length(), 3);
		message += (_users[i])->getUsername();
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
	if (_users.size() == 0)
	{
		handleFinishGame();
	}
	return false;
}

bool Game::handleAnswerFromUser(std::shared_ptr<User> user, int answerNo, int time)
{
	_currentTurnAnswers++;
	bool isCorrect = false;
	if (answerNo == _questions[_currQuestionIndex]->getCorrectAnswerIndex())
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
	for (int i = 0; i < _questions.size(); i++)
	{
		message += Helper::getPaddedNumber((_questions[i])->getQuestion().length(), 3);
		message += (_questions[i])->getQuestion();
	}
	_currentTurnAnswers = 0;
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
