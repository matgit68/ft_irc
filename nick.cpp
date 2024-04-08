#include "Server.hpp"

void nick(Client *client, std::string args) {
	// already checking with isValid()
	// if (args[0] == '#' || args[0] == '$' || args[0] == ':'
	// 		|| args.find_first_of(" ,*?!@.") != NPOS)
	// 	return ft_send(client->getFd(), ERR_ERRONEUSNICKNAME(client, args));
	
	std::string oldNick = client->getNick();
	std::string newNick = args;
	
	if (newNick.empty())
		return ft_send(client->getFd(), ERR_NONICKNAMEGIVEN(client));
	if (!client->getServer()->isNickAvailable(newNick))
		return ft_send(client->getFd(), ERR_NICKNAMEINUSE(client, newNick));
	if (!is_valid(newNick))
		return ft_send(client->getFd(), ERR_ERRONEUSNICKNAME(client, newNick));

	// nick can be empty ?
	if (oldNick.empty()) {  // changin the oldNick with the newNick
		client->setNick(newNick);
		std::cout << "Requesting the new nick \"" <<  newNick << "\"." << std::endl;
	}
	else {
		client->setOldNick(oldNick);
		client->setNick(newNick);
		std::cout << client->getOldNick() << " changed his nickname to " << newNick << "." << std::endl;
	}

	std::map<std::string, Channel*> chan = client->getServer()->getChannelMap();
	std::map<std::string, Channel*>::iterator it;
	std::set<int> dest;
	for (it = chan.begin(); it != chan.end(); it++) {
		if (it->second->getName() != ":" && it->second->isClient(client)) {
			std::set<int> tmp = it->second->getClientList();
			dest.insert(tmp.begin(), tmp.end());
		}
	}

	for (std::set<int>::iterator it = dest.begin(); it != dest.end(); it++)
		ft_send(*it, RPL_NICK(oldNick, client));
}
