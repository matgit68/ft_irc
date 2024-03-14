#include "Server.hpp"

void ping(Client *client, std::string args) {
	std::string answer;
	
	if (args.empty()) {
<<<<<<< HEAD
		sendClient(client->getFd(), ERR_NEEDMOREPARAMS( "PING"));
=======
		ft_send(client, "ERR_NEEDMOREPARAMS\r\n");
>>>>>>> 4a28f7aa2de2f57531a2b95d6520a0460f1ac100
		return ;
	}
	else
		answer = "PONG " + args + "\r\n";
	ft_send(client, answer);
}