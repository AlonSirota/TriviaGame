#include "DB.h"

DB::DB() : _db("serverDatabase.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE) //debugged
{
	_db.exec("pragma foreign_keys=ON");
	_db.exec("create table IF NOT EXISTS t_users (username string primary key not null, password string not null, email string not null)");
	_db.exec("create table IF NOT EXISTS t_games (game_id integer primary key autoincrement not null, status int not null, start_time datetime not null, end_time datetime)");
	_db.exec("create table IF NOT EXISTS t_questions (question_id integer primary key autoincrement not null, question string not null, correct_ans string not null, ans2 string not null, ans3 string not null, ans4 string not null)");
	_db.exec("create table IF NOT EXISTS t_players_answers (game_id integer not null, username string not null, question_id integer not null, player_answer string not null, is_correct integer not null, answer_time integer not null, primary key(game_id, username, question_id), foreign key(game_id) references t_games(game_id), foreign key(username) references t_users(username), foreign key(question_id) references t_questions(question_id))");
	//insertQuestion("what is the color of the sky?", "blue", "green", "red", "yellow");
	//insertQuestion("3+7=...", "10", "9", "11", "420");
	//insertQuestion("8:2=...", "4", "3", "2", "425");
}

//TODO prevent SQL injections.
bool DB::isUserExist(std::string username) //debugged
{
	SQLite::Statement query(_db, "SELECT COUNT(*) FROM t_users WHERE username = ?");
	query.bind(1, username);

	try
	{
		query.executeStep();
		int count = query.getColumn(0);

		if (count == 0) return false;
		else return true;
	}
	catch (const SQLite::Exception& exception)
	{
		std::cout << "isUserExist failed: " << exception.what() << "\n";
	return false;
	}
}

//TODO prevent SQL injections.
bool DB::addNewUser(std::string username, std::string password, std::string email) //debugged
{
	if (isUserExist(username)) return false;
	else
	{
		SQLite::Statement query(_db, "INSERT INTO t_users (username, password, email) values (?, ?, ?)");
		query.bind(1, username);
		query.bind(2, password);
		query.bind(3, email);
		query.exec();
		return true;
}
}

bool DB::isUserAndPassMatch(std::string username, std::string password) //debugged
{
	SQLite::Statement query(_db, "SELECT COUNT(*) FROM t_users WHERE username=? AND password=?");
	query.bind(1, username);
	query.bind(2, password);

	query.executeStep();
	
	if (atoi(query.getColumn(0)) == 0) return false;
	else return true;
}

std::string DB::columnToString(SQLite::Column c)
{
	std::string temp = c;
	return temp;
}

//helper class for easier DB management, maybe use for a new feature?
void DB::insertQuestion(std::string question, std::string correctAns, std::string ans1, std::string ans2, std::string ans3)
{
	try
	{
																		//id is autoincrement
		SQLite::Statement query(_db, "INSERT INTO t_questions (question, correct_ans, ans2,ans3, ans4) VALUES (?, ?, ?, ?, ?)");
		query.bind(1, question);
		query.bind(2, correctAns);
		query.bind(3, ans1);
		query.bind(4, ans2);
		query.bind(5, ans3);
		query.exec();
	}
	catch (const SQLite::Exception& e)
	{
		std::cout << "insertQuestion failed: " << e.what() << "\n";
	}
}

//throws std::exception if db doesn't contain enough questions.
std::vector<std::shared_ptr<Question>> DB::questionInit(int amount) //debugged.
{
	std::vector<std::shared_ptr<Question>> questions;
	std::string question, correctAns, a2, a3, a4;

	SQLite::Statement checkAmount(_db, "SELECT COUNT(*) FROM t_questions");
	checkAmount.executeStep();
	if (atoi(checkAmount.getColumn(0)) < amount)
	{
		throw std::exception("database doesn't contain enough questions");
	}
	//else:

	//selects 'amount' of random questions.
	SQLite::Statement query(_db, "SELECT question_id, question, correct_ans, ans2, ans3, ans4 FROM t_questions ORDER BY random() LIMIT ?");
	query.bind(1, amount);
	
	while (query.executeStep()) //creates a question and adds it to vector for each question.
	{
		int id = query.getColumn(0);
		std::string question = query.getColumn(1);
		std::string correctAns = query.getColumn(2);
		std::string a2 = query.getColumn(3);
		std::string a3 = query.getColumn(4);
		std::string a4 = query.getColumn(5);

		std::shared_ptr<Question> temp = std::make_shared<Question>(id, question, correctAns, a2, a3, a4);//(new Question(id, question, correctAns, a2, a3, a4));
		questions.push_back(temp);
	}

	return questions;
}

/*
	Input: amount of high scores to return.
	Output: a vector, each element is a pair, first = username, second = score.
	Note: The vector will only contain as many elements as high scores in DB.
*/
std::vector<std::pair<std::string, std::string>> DB::getBestScores(int amount)
{
	std::vector<std::pair<std::string, std::string>> scores;

	//groups rows with the same username and game_id, select the username and the score (sum of is_correct=1), selects in descending order.
	SQLite::Statement query(_db, "SELECT username, SUM(is_correct) FROM t_players_answers GROUP BY username ORDER BY SUM(is_correct) DESC LIMIT ?");
	query.bind(1, amount); //binds the limit value

		while (query.executeStep())
		{
		std::string username = query.getColumn(0);
		std::string score = query.getColumn(1);
		scores.push_back(std::pair<std::string, std::string>(username, score));
	}

	return scores;
		}

//see enum in DB.h. THE MESSAGE ISN'T PREPARED YET - need more parsing.
std::vector<std::string> DB::getPersonalStatus(std::string username)
{
	std::vector<std::string> vector(4);
	SQLite::Statement query(_db, "SELECT COUNT(DISTINCT game_id), SUM(is_correct), AVG(answer_time) FROM t_players_answers WHERE username=?");
	query.bind(1, username); //binds to the WHERE username=?

	query.executeStep();
	vector[_personalStatusIndexes::NUMBER_OF_GAMES] = columnToString(query.getColumn(0));
	vector[_personalStatusIndexes::NUMBER_OF_RIGHT_ANWERS] = columnToString(query.getColumn(1));
	vector[_personalStatusIndexes::AVG_TIME_FOR_ANS] = columnToString(query.getColumn(2));

	SQLite::Statement query2(_db, "SELECT COUNT(*) FROM t_players_answers WHERE username=? AND is_correct=0");
	query2.bind(1, username);

	query2.executeStep();
	vector[_personalStatusIndexes::NUMBER_OF_WRONG_ANSWERS] = columnToString(query2.getColumn(0));

	return vector;
	}

int DB::insertNewGame() //this isn't thread safe!!! debugged.
	{
	_db.exec("INSERT INTO t_games (start_time, status, end_time) values (datetime('now', 'localtime'), 0, null)");
	return _db.execAndGet("SELECT last_insert_rowid()");
	}

bool DB::updateGameStatus(int gameId) //debugged
{
	SQLite::Statement query(_db, "UPDATE t_games SET status=1, end_time=datetime('now','localtime') WHERE game_id=?");
	query.bind(1, gameId);
	if (query.exec()) return true; //exec returns the amount of rows modified, returns true if that number isn't 0.
	else return false;
}

//t_players_answers columns:game_id integer, username string, question_id integer, player_answer string, is_correct integer, answer_time integer
bool DB::addAnswerToUser(int gameId, std::string username, int questionId, std::string answer, bool isCorrect, int answerTime_seconds)
{
	try
	{
		/*SQLite::Statement query2(_db, "PRAGMA table_info(t_players_answers)");
		query2.executeStep();
		std::cout << columnToString(query2.getColumn(0)) << std::endl;
		std::cout << columnToString(query2.getColumn(1)) << std::endl;
		std::cout << columnToString(query2.getColumn(2)) << std::endl;
		std::cout << columnToString(query2.getColumn(3)) << std::endl;
		std::cout << columnToString(query2.getColumn(4)) << std::endl;*/
		SQLite::Statement query2(_db, "INSERT INTO test3(game_id,username) values(2,'user')");
		try
		{
			query2.executeStep();
			//return true;
		}
		catch (const SQLite::Exception &e)
		{
			std::cout << "addAnswerToUser failed: " << e.what() << "\n"; //TODO delete this, just for debugging.
			return false;
		}
		SQLite::Statement query3(_db, "INSERT INTO test2 VALUES(81,'user2',3,'Movie file',0,10);");
		////////////////////////
		SQLite::Statement query(_db, "INSERT INTO t_players_answers(game_id, username, question_id, player_answer, is_correct, answer_time) values(?, ?, ?, ?, ?, ?)");
		query.bind(1, gameId);
		query.bind(2, username);
		query.bind(3, questionId);
		query.bind(4, answer);
		if (isCorrect)
		{
			query.bind(5, 1);
		}
		else
		{
			query.bind(5, 0);
		}
		query.bind(6, answerTime_seconds);

		try
		{
			query.executeStep();
			return true;
		}
		catch (const SQLite::Exception &e)
		{
			std::cout << "addAnswerToUser failed: " << e.what() << "\n"; //TODO delete this, just for debugging.
			return false;
		}
	}
	catch (SQLite::Exception & e)
	{
		std::cout << _db.getExtendedErrorCode() << std::endl;
		std::cout << e.what() << std::endl;
	}
	
}
