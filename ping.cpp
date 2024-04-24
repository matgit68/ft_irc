#include "Server.hpp"

void ping(Client *client, std::string args) {
	std::string answer;
	
	if (args.empty())
		return client->getServer()->ft_send(client->getFd(), "ERR_NEEDMOREPARAMS\r\n");
	else
		answer = "PONG " + args + "\r\n";
	client->getServer()->ft_send(client->getFd(), answer);
}