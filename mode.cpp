#include "Server.hpp"

void mode(Client *client, std::string args) {
	std::cout << args << std::endl;
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
		chan->sendClients(RPL_CHANNELMODEIS(client->getNick(), chan->getName(), chan->getMode()));
		chan->sendOps(RPL_CHANNELMODEISWITHKEY(client->getNick(), chan->getName(), chan->getMode(), chan->getPasswd()));
		return ;
	}

	if ((space = args.find(' ')) != NPOS) { // recovering modestring (eg -k or +it) and modeargs
		modestring = args.substr(0, space);
		modeargs = args.substr(space + 1, args.size());
	}
	else // only modestring
		modestring = args.substr(0, args.size());

	// if (modestring.find_first_not_of("+-itklo") != NPOS)
	// 	return ft_send(client->getFd(), ERR_UMODEUNKNOWNFLAG());

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
		if (modeset == '+') {
			puts("add mode");
			chan->addMode(client, modestring[0], modeargs);
		}
		if (modeset == '-') {
			puts("remove mode");
			chan->unMode(client, modestring[0], modeargs);
		}
		modestring.erase(0, 1);
	}
}