#include "DB.h"

DB::DB() : _db("serverDatabase.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE)
{
	_db.exec("pragma foreign_keys=1");
	_db.exec("create table t_users (username string primary key not null, password string not null, email string not null)");
	_db.exec("create table t_games (game_id integer primary key autoincrement not null, start_time datetime not null, end_time datetime)");
	_db.exec("create table t_questions (question_id integer primary key autoincrement not null, question string not null, correct_ans string not null, ans2 string not null, ans3 string not null, ans4 string not null)");
	_db.exec("create table t_player_answers (game_id integer not null, username string not null, question_id integer not null, player_answer string not null, is_correct integer not null, answer_time integer not null, primary key(game_id, username, question_id), foreign key(game_id) references t_games(game_id), foreign key(username) references t_users(username), foreign key(question_id) references t_questions(question_id))");
}

//TODO prevent SQL injections.
bool DB::isUserExist(std::string username)
{
	SQLite::Statement query(_db, "SELECT COUNT(*) FROM t_users WHERE username = ?");
	query.bind(1, username);

	try
	{
		query.exec();
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

int DB::insertNewGame()
{
	return 0;
}

bool DB::updateGameStatus(int)
{
	return false;
}

bool DB::addAnswerToUser(int, std::string, int, std::string, bool, int)
{
	return false;
}
void DB::example()
{
	try
	{
		// Open a database file
		SQLite::Database    db("example.db3", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

		// Create a new table with an explicit "id" column aliasing the underlying rowid
		db.exec("DROP TABLE IF EXISTS test"); // just good practice in any case you don't have to do it
		db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT, size integer)");

		// first row
		int nb = db.exec("INSERT INTO test VALUES (1,\"test1\", 20)"); // pay attention to the escaping 
		nb = db.exec("INSERT INTO test VALUES (2,\"test2\", 2)"); // pay attention to the escaping 
		nb = db.exec("INSERT INTO test VALUES (3,\"test\", 7)"); // pay attention to the escaping 

																 // Compile a SQL query, containing one parameter (index 1)
		SQLite::Statement   query(db, "SELECT * FROM test WHERE size > ?");

		// Bind the integer value 6 to the first parameter of the SQL query
		query.bind(1, 6);

		// Loop to execute the query step by step, to get rows of result
		while (query.executeStep())
		{
			// Demonstrate how to get some typed column value
			int         id = query.getColumn(0);
			const char* value = query.getColumn(1);
			int         size = query.getColumn(2);

			std::cout << "row: " << id << ", " << value << ", " << size << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
	std::getchar();
}

std::vector<std::shared_ptr<Question>> DB::questionInit(int)
{
	return std::vector<std::shared_ptr<Question>>();
}
