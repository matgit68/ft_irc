#include "Server.hpp"

void cap(Client *client, std::string args) {
	std::string str = takeNextArg(args);
	std::cout << "CAP arg: " << str << std::endl;
	if (str.compare("LS") == 0)
		ft_send(client->getFd(), RPL_CAPLS(client->getServer()->getHostname()));
	if (str.compare("END") == 0)
		ft_send(client->getFd(), client->getServer()->getHostname());
}