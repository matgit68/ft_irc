#include "Server.hpp"

void ping(Client *client, std::string args) {
	std::string answer;
	if (args.empty())
		return ;
	else
		answer = "PONG " + args + "\r\n";
	send(client->getFd(), answer.c_str(), answer.size(), 0);
}