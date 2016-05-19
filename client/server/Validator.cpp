#include "Validator.h"

static bool isPasswordValid(std::string pass)
{
	std::regex reg = std::regex();

	if (pass.length() < 4) //less than 4 chars
	{
		return false;
	}

	if (pass.find(" ") != -1) //contains spaces
	{
		return false;
	}
}

static bool isUsernameValid(std::string)
{

}