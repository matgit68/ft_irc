#include "Server.hpp"

//With nc it doesnt give the reason but with irssi it gives

std::string parseReason(std::string msg)
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
	std::string nick = client->getNick();
	std::string reason = parseReason(args);

	if(reason.empty())
		reason = "No reason given";
	std::map<std::string, Channel*> channels = client->getServer()->getChannelMap();
	std::map<std::string, Channel*>::iterator it = channels.find(chan);

	if(it == channels.end()) //checking if the channel exists
		return server->ft_send(client->getFd(), ERR_NOSUCHCHANNEL(client, chan));
	else if(!server->getClient(client->getFd())) //checking if the client in the channel
		server->ft_send(client->getFd(), ERR_NOTONCHANNEL(chan));
	else
	{
		it->second->sendChan(NULL, RPL_PART(client, chan, reason));
		it->second->removeUser(client);
	}
	server->checkEmptyChannels();
}