#include "Server.hpp"

void mode(Client *client, std::string args) {
	char modeset = 0;
	std::string modestring, modeargs;

	size_t space = args.find(' ');
	if ((space = args.find(' ')) == NPOS)
		space = args.size();

	Channel *chan = client->getServer()->getChannel(args.substr(0, space)); // recovering channel (eg #channel)
	if (chan == NULL)
		return ft_send(client->getFd(), ERR_NOSUCHCHANNEL(args.substr(0, space)));
	args.erase(0, space + 1);

	// no args => mode status request
	if (args.empty()) {
		if (chan->isClient(client))
			chan->sendChan(0, RPL_CHANNELMODEISWITHKEY(client->getNick(), chan->getName(), chan->getMode(), chan->getPasswd()));
		else
			ft_send(client->getFd(), RPL_CHANNELMODEISWITHKEY(client->getNick(), chan->getName(), chan->getMode(), "[key]"));
		return ;
	}

	if ((space = args.find(' ')) != NPOS) { // recovering modestring (eg -k or +it) and modeargs
		modestring = args.substr(0, space);
		modeargs = args.substr(space + 1, args.size());
	}
	else // only modestring, no modeargs
		modestring = args.substr(0, args.size());

	// handling each modestring one by one and taking arguments in modeargs in the same order
	while (!modestring.empty()) {
		if (!chan->isOp(client->getFd())) // checking client op privileges
			return ft_send(client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNick(), chan->getName()));
		while (modestring[0] == '+' || modestring[0] == '-') {
			modeset = modestring[0];
			modestring.erase(0, 1);
		}
		if (!modeset)
			return ft_send(client->getFd(), ERR_UMODEUNKNOWNFLAG());
		if (modeset == '+')
			chan->addMode(client, modestring[0], modeargs);
		if (modeset == '-')
			chan->unMode(client, modestring[0], modeargs);
		modestring.erase(0, 1);
	}
	if (chan->isClient(client)) {
		std::cout << "Sending all clients" << std::endl;
		chan->sendChan(0, RPL_CHANNELMODEISWITHKEY(client->getNick(), chan->getName(), chan->getMode(), chan->getPasswd()));
	}
	else {
		ft_send(client->getFd(), RPL_CHANNELMODEISWITHKEY(client->getNick(), chan->getName(), chan->getMode(), "[key]"));
		std::cout << "Sending only to origin" << std::endl;
	}
}