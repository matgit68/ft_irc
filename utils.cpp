#include "hpp.hpp"

void ft_send(int fd, std::string msg) {
	if (msg.find("PING") == NPOS && msg.find("PONG") == NPOS)
		std::cout << "Sent(" << fd << ") : " << msg << std::endl;
	send(fd, msg.c_str(), msg.size(), 0);
}

void dispChanList(Client *client, std::string str) {
	(void) str;
	client->getServer()->dispChannels(client);
}

bool is_valid(const std::string nickname){
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