#include "Validator.h"

bool Validator::isPasswordValid(std::string pass)
{
	bool shorterThan4 = pass.length() < 4,
		containsSpace = pass.find(" ") != -1,
		containsNumber = std::regex_match(pass, std::regex("[0-9]")),
		containsLowerCase = std::regex_match(pass, std::regex("[a-z]")),
		containsUpperCase = std::regex_match(pass, std::regex("[A-Z]"));

	if (shorterThan4 || containsSpace || !containsNumber || !containsLowerCase || !containsUpperCase) return false;
	else return true;
}

bool Validator::isUsernameValid(std::string user)
{
	bool startsWithLetter = std::regex_search(user, std::regex("^[a-z|A-Z]")),
		containsSpace = std::regex_match(user, std::regex(" ")),
		isEmpty = user == "";

	if (!startsWithLetter || containsSpace || isEmpty)
	{
		return false;
	}
	else
	{
		return true;
	}
}