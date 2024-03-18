#include "hpp.hpp"

void ft_send(int fd, std::string msg) {
	if (msg.find("PING") == std::string::npos && msg.find("PONG") == std::string::npos)
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
		if(i > 1 && validChars.find(c) == std::string::npos)
			return false;
	}
	return true;
}
