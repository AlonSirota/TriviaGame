#include "DB.h"

DB::DB()
{
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
