#include "Server.hpp"

//With nc it doesnt give the reason but with irssi it gives

static std::string parseReason(std::string msg)
{
	std::string reason;
	reason.clear();

	if(msg.find(":") != msg.NPOS)
		reason.append(msg, msg.find(":") + 1, NPOS);
	return reason;
}

void part(Client *client, std::string args) {
	Server *server = client->getServer();
	std::string chan = takeNextArg(args);
	std::string msg = takeNextArg(args);
	std::string nick = client->getNick();
	std::string reason;
	
	if(msg.empty())
		reason = "No reason given";
	else
		reason = parseReason(msg);
	std::map<std::string, Channel*> channels = client->getServer()->getChannelMap();
	std::map<std::string, Channel*>::iterator it = channels.find(chan);

	if(it == channels.end()) //checking if the channel exists
		return ft_send(client->getFd(), ERR_NOSUCHCHANNEL(chan));
	else if(!server->getClient(client->getFd())) //checking if the client in the channel
		ft_send(client->getFd(), ERR_NOTONCHANNEL(chan));
	else
	{
		it->second->removeUser(client); 
		ft_send(client->getFd(), RPL_PART(nick, chan, reason));
	}	
}


