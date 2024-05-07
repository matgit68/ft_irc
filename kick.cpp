#include "Server.hpp"

void kick(Client *client, std::string args) {
	Server *server = client->getServer();
	if (args.empty())
		return server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("KICK"));
	std::string chanName, users, reason;
	chanName = takeNextArg(args);
	if (args.empty())
		return server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("KICK"));
	users = takeNextArg(args);
	if (users.empty())
		return server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("KICK"));

	Channel *chan = client->getServer()->getChannel(chanName);
	if (!chan)
		return server->ft_send(client->getFd(), ERR_NOSUCHCHANNEL(client, chanName));
	if (!chan->isClient(client))
		return server->ft_send(client->getFd(), ERR_NOTONCHANNEL(chanName));
	if (!chan->isOp(client->getFd()))
		return server->ft_send(client->getFd(), ERR_CHANOPRIVSNEEDED(client, chan));

	Client *target;
	std::string nick;
	while (!users.empty()) {
		nick = takeNextArg(',', users);
		target = client->getServer()->getClient(nick);
		if (!target) {
			std::cout << "Target _" << nick << "_ not found" << std::endl;
			server->ft_send(client->getFd(), ERR_USERNOTINCHANNEL(client, nick, chanName));
			continue;
		}
		if (!chan->isClient(target))
			server->ft_send(client->getFd(), ERR_USERNOTINCHANNEL(client, nick, chanName));
		else {
			chan->sendChan(NULL, RPL_KICK(client, chanName, target, args));
			chan->removeUser(target);
		}
	}
	client->getServer()->checkEmptyChannels();
}