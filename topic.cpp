#include "Server.hpp"


void topic(Client *client, std::string args) {

	Server *server = client->getServer();
	Channel *channel;
	std::string chan = takeNextArg(args);
	std::string topic = takeNextArg(args); //get the topic
	if(topic[0] == ':')
		topic.erase(0, 1);
	channel = server->getChannel(chan);

	if(!server->findChannel(chan)) //checking if the channel exists
	{
		return ft_send(client, ERR_NOSUCHCHANNEL(chan));
	}
	
	if(!server->getClient(client->getFd()))
		ft_send(client, ERR_NOTONCHANNEL(client->getNick(), chan));
	
 	if(topic.empty())
	{
		if(channel->getTopic().empty())
			ft_send(client, RPL_NOTOPIC(client->getNick(), chan));
		else
			ft_send(client, RPL_TOPIC(client->getNick(), chan, channel->getTopic()));
	}
	else
	{
	 if (channel->getMode().find('t') != NPOS && channel->isOp(client->getFd()) == false)
	 	ft_send(client, ERR_CHANOPRIVSNEEDED(client->getNick(), chan));
	else
		channel->setTopic(topic);
	}
}

