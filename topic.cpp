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
		return ft_send(client->getFd(), ERR_NOSUCHCHANNEL(chan));
	}
	
	if(!server->getClient(client->getFd()))
		ft_send(client->getFd(), ERR_NOTONCHANNEL(chan));
	
 	if(topic.empty())
	{
		if(channel->getTopic().empty())
			ft_send(client->getFd(), RPL_NOTOPIC(client, channel));
		else
			ft_send(client->getFd(), RPL_TOPIC(client, channel));
	}
	else
	{
	 if (channel->getMode().find('t') != NPOS && !channel->isOp(client->getFd())) {
	 	ft_send(client->getFd(), ERR_CHANOPRIVSNEEDED(client, channel));
		return;
	 }
	 else
	 {
		channel->setTopic(topic);
		server->broadcast(client, RPL_TOPIC(client, channel));
		channel->sendOps(RPL_TOPIC(client, channel));
	 }
	
	}
	
}

//when non-operator user tries to change the topic, he is kicked out from the channel, 
//but it should send just ERR_CHANOPRIVSNEEDED()


