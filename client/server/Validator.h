#include <string>
#include <regex>

class Validator
{
public:
	static bool isPasswordValid(std::string);
	static bool isUsernameValid(std::string);
};