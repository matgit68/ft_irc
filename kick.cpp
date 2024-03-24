#include "Server.hpp"

void kick(Client *client, std::string args) {
	if (args.empty()) {
		ft_send(client->getFd(), ERR_NEEDMOREPARAMS("KICK"));
		return ;
	}

	std::string chanName, reason;
	size_t	pos = args.find_first_of(" ");
	if (pos == std::string::npos) {
		ft_send(client->getFd(), ERR_NEEDMOREPARAMS("KICK"));
		return ;
	}
	chanName = args.substr(0, pos);
	args.erase(0, pos + 1);
	if (args.empty()) {
		ft_send(client->getFd(), ERR_NEEDMOREPARAMS("KICK"));
		return ;
	}

	pos = args.find_first_of(" ");
	if (pos != std::string::npos) {
		reason = args.substr(pos + 1, args.size());
		args.erase(pos +1, std::string::npos);
	}
	else
		reason = "I'm the boss.";

	Channel *chan = client->getServer()->getChannel(chanName);
	if (!chan) {
		ft_send(client->getFd(), ERR_NOSUCHCHANNEL(chanName));
		return ;
	}
	if (!chan->isOp(client->getFd())) {
		ft_send(client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNick(), chanName));
		return ;
	}
	if (!chan->isClient(client)) {
		ft_send(client->getFd(), ERR_NOTONCHANNEL(chanName));
		return ;
	}
	Client *target;
	std::string nick;
	while (!args.empty()) {
		nick = args.substr(0, args.find_first_of(","));
		args.erase(0, args.find_first_of(","));
		target = client->getServer()->getClient(nick);
		if (!target)
			ft_send(client->getFd(), ERR_USERNOTINCHANNEL(client->getNick(), nick, chanName)); //to confirm 		//	ft_send(client->getFd(), ERR_NOSUCHNICK(client->getNick(), nick));
		else
		{
			if (!chan->isClient(target))
				ft_send(client->getFd(), ERR_USERNOTINCHANNEL(client->getNick(), nick, chanName));
			else
			{
				if (chan->isOp(target->getFd()))
					chan->removeOp(target->getFd()); // to confirm
				chan->sendChan(client, RPL_KICK(client->getNick(), chanName, target->getNick(), reason));
				chan->delClient(target);
			}
		}
	}
}