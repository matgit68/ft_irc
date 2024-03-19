#include "Server.hpp"

void mode(Client *client, std::string args) {
	std::cout << args << std::endl;
	size_t pos = args.find(' ');
	char modeset = 0;
	std::string modestring, modeargs;

	// if (pos == NPOS)
	// 	return ft_send(client->getFd(), ERR_NEEDMOREPARAMS("MODE1"));

	Channel *chan = client->getServer()->getChannel(args.substr(0, pos)); // recovering channel (eg #channel)
	if (chan == NULL)
		return ft_send(client->getFd(), ERR_NOSUCHCHANNEL(args.substr(0, pos)));
	args.erase(0, pos + 1);

	if ((pos = args.find(' ')) == NPOS) // recovering modestring (eg -k or +it)
		pos = args.size();
	modestring = args.substr(0, pos);
	if (modestring.empty())
		return ft_send(client->getFd(), RPL_UMODEIS(client->getNick(), chan->getMode())); // !! Marche pas ici
	if (modestring.find_first_not_of("+-itklo") != NPOS)
		return ft_send(client->getFd(), ERR_UMODEUNKNOWNFLAG());
	args.erase(0, pos + 1);

	while (!modestring.empty()) { // handling each modestring one by one and taking arguments in the same order
		if (!chan->isOp(client->getFd())) // checking client op privileges
			return ft_send(client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNick(), chan->getName()));
		if (modestring[0] == '+' || modestring[0] == '-') {
			modeset = modestring[0];
			modestring.erase(0, 1);
		}
		if (!modeset)
			return ft_send(client->getFd(), ERR_UMODEUNKNOWNFLAG());
		if (modestring[0] == 'i') {
			if (modeset == '+')
				chan->addMode('i');
			if (modeset == '-')
				chan->unMode('i');
		}
		if (modestring[0] == 't') {
			if (modeset == '+')
				chan->addMode('t');
			if (modeset == '-')
				chan->unMode('t');
		}
		if (modestring[0] == 'k') {
			if (modeset == '+') {
				if ((pos = args.find(' ')) != NPOS) { // getting next argument (password)
					modeargs = args.substr(0, pos);
					args.erase(0, pos + 1);
					chan->addMode('k', modeargs);
				}
			}
			if (modeset == '-')
				chan->unMode('k');
		}
		if (modestring[0] == 'l') {
			if (modeset == '+') {
				if ((pos = args.find(' ')) != NPOS) { // getting next argument (limit)
					modeargs = args.substr(0, pos);
					args.erase(0, pos + 1);
					chan->addMode('l', atoi(modeargs.c_str()));
				}
			}
			if (modeset == '-')
				chan->unMode('l');
		}
		if (modestring[0] == 'o') {
			if (modeset == '+') {
				if ((pos = args.find(' ')) != NPOS) { // getting next argument (clientname)
					modeargs = args.substr(0, pos);
					args.erase(0, pos + 1);
					chan->addMode('o', modeargs);
				}
			}
			if (modeset == '-')
				chan->unMode('o');
		}
		modestring.erase(0, 1);
	}
}