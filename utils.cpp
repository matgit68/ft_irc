#include "hpp.hpp"

void ft_send(Client *client, std::string msg) {
	std::cout << "Sent(" << client->getFd() << ") : " << msg << std::endl;
	send(client->getFd(), msg.c_str(), msg.size(), 0);
}

void dispChanList(Client *client, std::string str) {
	(void) str;
	client->getServer()->dispChannels(client);
}
