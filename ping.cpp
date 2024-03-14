#include "Server.hpp"

void ping(Client *client, std::string args) {
	std::string answer;
	
	if (args.empty()) {
		ft_send(client, "ERR_NEEDMOREPARAMS\r\n");
		return ;
	}
	else
		answer = "PONG " + args + "\r\n";
	ft_send(client, answer);
}