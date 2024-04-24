#include "Server.hpp"


void cap(Client *client, std::string args) {
	(void) client;
	(void) args;
	// std::string str = takeNextArg(args);
	// if (str.compare("LS") == 0)
	// 	client->getServer()->ft_send(client->getFd(), RPL_CAPLS(client));
	// if (str.compare("END") == 0)
	// 	_server->ft_send(client->getFd(), RPL_CAPEND(client->getServer()->getHostname()));
}