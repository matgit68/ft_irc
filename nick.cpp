#include "Server.hpp"

void nick(Client *client, std::string args) {
	if (args[0] == '#' || args[0] == '$' || args[0] == ':'
			|| args.find_first_of(" ,*?!@.") != NPOS)
		return ft_send(client->getFd(), ERR_ERRONEUSNICKNAME(client, args));
	
	
	std::string oldNick = client->getNick();
	std::string newNick = args;
	
	if(newNick.length() == 0){
		ft_send(client->getFd(), ERR_NONICKNAMEGIVEN(client));
		return;
	}
	if(!client->getServer()->isNickAvailable(newNick)) //checking if there is already that nick
	{
			ft_send(client->getFd(), ERR_NICKNAMEINUSE(client, newNick));
			return ;
	} 
	if(!is_valid(newNick)) //is_valid() func is in the utils.cpp
	{
		ft_send(client->getFd(), ERR_ERRONEUSNICKNAME(client, newNick));
		return;
	}
	if(oldNick.empty()) {  //changin the oldNick with the newNick
		client->setNick(newNick);
		std::cout << "Requesting the new nick \"" <<  newNick << "\"." << std::endl;
	}else {
		client->setOldNick(oldNick);
		client->setNick(newNick);
		std::cout << client->getOldNick() << " changed his nickname to " << newNick << "." << std::endl;
	}

	std::map<std::string, Channel*> chan = client->getServer()->getChannelMap();
	std::set<int>	dest;
	for (std::map<std::string, Channel*>::iterator it = chan.begin(); it != chan.end(); it++)
	{
		if (it->second->getName() != ":" && it->second->isClient(client))
		{
			std::set<int> tmp = it->second->getClientList();
			for (std::set<int>::iterator add = tmp.begin(); add != tmp.end(); add++)
				dest.insert(*add);
		}
	}
	for (std::set<int>::iterator it = dest.begin(); it != dest.end(); it++)
		ft_send(*it, RPL_NICK(oldNick, newNick, client));
	client->getServer()->broadcast(client,RPL_NICK(oldNick, newNick, client));
}
	