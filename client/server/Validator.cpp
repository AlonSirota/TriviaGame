#include "Validator.h"

static bool isPasswordValid(std::string pass)
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

static bool isUsernameValid(std::string user)
{
	if (std::regex_match(user, "^[a-z]"))
}