#include "Server.hpp"

void part(Client *client, std::string args) {
	Server *server = client->getServer();
	std::string chans = takeNextArg(args);
	std::string chanName;
	Channel *channel;

	if (args[0] == ':')
		args.erase(0, 1);

	while (!chans.empty()) {
		chanName = takeNextArg(',', chans);
		channel = server->getChannel(chanName);
		if (channel == NULL)
			server->ft_send(client->getFd(), ERR_NOSUCHCHANNEL(client, chanName));
		else if(!channel->isClient(client)) //checking if the client in the channel
			server->ft_send(client->getFd(), ERR_NOTONCHANNEL(channel->getName()));
		else {
			channel->sendChan(NULL, RPL_PART(client, channel->getName(), args));
			channel->removeUser(client);
		}
		server->checkEmptyChannels();
	}
}