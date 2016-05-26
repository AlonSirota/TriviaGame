#include "Validator.h"

bool Validator::isPasswordValid(std::string pass)
{
	if (pass.length() < 4 ||
		pass.find(" ") != -1 ||
		std::regex_match(pass, std::regex("[0-9]")) == 0 ||
		std::regex_match(pass, std::regex("[a-z]")) == 0 ||
		std::regex_match(pass, std::regex("[A-Z]")) == 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Validator::isUsernameValid(std::string user)
{
	if (std::regex_match(user, std::regex("^[a-z|A-Z]")) == 0 ||
		std::regex_match(user, std::regex(" ")) != 0 ||
		user == "")
	{
		return false;
	}
	else
	{
		return true;
	}
}