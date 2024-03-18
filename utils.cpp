#include "Server.hpp"

void ft_send(Client *client, std::string msg) {
	std::cout << "Sent(fd" << client->getFd() << ") : " << msg << std::endl;
	send(client->getFd(), msg.c_str(), msg.size(), 0);
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