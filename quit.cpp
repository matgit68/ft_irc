#include "Server.hpp"

void quit(Client *client, std::string args) {
	//Server *server = client->getServer();
	std::string reason = parseReason(args);
	std::string nick = client->getNick();

//this loop doesnt work it sends 2 times to itself the msg, we need it to be in channel the msg
	std::map<std::string, Channel*> chan = client->getServer()->getChannelMap();
	std::set<int>	dest;
	for (std::map<std::string, Channel*>::iterator it = chan.begin(); it != chan.end(); it++)
	{
		std::set<int> tmp = it->second->getClientList();
		dest.insert(tmp.begin(), tmp.end());
	}
	
	for (std::set<int>::iterator it = dest.begin(); it != dest.end(); it++)
	{
		ft_send(client->getFd(), RPL_QUIT(client, reason));
	}	 
}
