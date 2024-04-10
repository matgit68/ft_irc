#include "Server.hpp"

void kick(Client *client, std::string args) {
	if (args.empty())
		return ft_send(client->getFd(), ERR_NEEDMOREPARAMS("KICK"));
	std::string chanName, users, reason;
	chanName = takeNextArg(args);
	if (args.empty())
		return ft_send(client->getFd(), ERR_NEEDMOREPARAMS("KICK"));
	users = takeNextArg(args);
	if (users.empty())
		return ft_send(client->getFd(), ERR_NEEDMOREPARAMS("KICK"));

	Channel *chan = client->getServer()->getChannel(chanName);
	if (!chan)
		return ft_send(client->getFd(), ERR_NOSUCHCHANNEL(chanName));
	if (!chan->isClient(client))
		return ft_send(client->getFd(), ERR_NOTONCHANNEL(chanName));
	if (!chan->isOp(client->getFd()))
		return ft_send(client->getFd(), ERR_CHANOPRIVSNEEDED(client, chan));

	Client *target;
	std::string nick;
	while (!users.empty()) {
		nick = takeNextArg(',', users);
		std::cout << "Nick -> [" << nick << "]"<< std::endl;
		target = client->getServer()->getClient(nick);
		if (!target) {
			std::cout << "Target _" << nick << "_ not found" << std::endl;
			ft_send(client->getFd(), ERR_USERNOTINCHANNEL(client, nick, chanName)); //to confirm //	ft_send(client->getFd(), ERR_NOSUCHNICK(client->getNick(), nick)
			continue;
		}
		if (!chan->isClient(target))
			ft_send(client->getFd(), ERR_USERNOTINCHANNEL(client, nick, chanName));
		else {
			chan->sendChan(NULL, RPL_KICK(client, chanName, target, args));
			chan->removeUser(target); // to confirm
		}
	}
	client->getServer()->checkEmptyChannels();
}