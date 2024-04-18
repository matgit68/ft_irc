#include "Server.hpp"

void who(Client *client, std::string args) {
	Server *server = client->getServer();
	Channel * channel;

	if (args.empty())
		return server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("WHO"));
	
	channel = server->getChannel(args);
	if (channel == NULL)
		return server->ft_send(client->getFd(), ERR_NOSUCHCHANNEL(client, args));
	
	server->ft_send(client->getFd(), RPL_NAMREPLY(client, channel, channel->getClientListbyName()));
	server->ft_send(client->getFd(), RPL_ENDOFNAMES(client, channel));
}