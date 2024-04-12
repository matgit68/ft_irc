#include "hpp.hpp"

void dispChanList(Client *client, std::string str) {
	(void) str;
	client->getServer()->dispChannels(client);
}

bool is_valid(const std::string nickname){
	if (nickname[0] == '#' || nickname[0] == '$' || nickname[0] == ':'
			|| nickname.find(" ,*?!@.") != NPOS)
		return false;
	if(nickname.length() < 1 || nickname.length() > 32)
		return false;
	const std::string validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_[]{}\\|";
	for (size_t i = 0; i < nickname.length(); i++)
	{
		char c = nickname[i];
		if(i == 0 && !isalpha(c))
			return false;
		if(i > 1 && validChars.find(c) == NPOS)
			return false;
	}
	return true;
}

std::string takeNextArg(std::string &str) { // USELESS ! -> iStringStream 
	std::string res;
	size_t space = str.find(' ');
	if (space == NPOS) {
		res = str;
		str.clear();
	}
	else {
		res = str.substr(0, space);
		str.erase(0, space + 1);
	}
	return res;
}

std::string takeNextArg(char sep, std::string &str) { // USELESS ! -> iStringStream 
	std::string res;
	size_t space = str.find(sep);

	if (space == NPOS) {
		res = str;
		str.clear();
	}
	else {
		res = str.substr(0, space);
		str.erase(0, space + 1);
	}
	return res;
}

void trim(std::string &str) {
	while(std::isspace(str[0]))
		str.erase(0, 1);
	while(std::isspace(str[str.size()]))
		str.erase(str.size() - 1, 1);
}