#include "Server.hpp"

void topic(Client *client, std::string args) {

	Server *server = client->getServer();
	Channel *channel;
	std::string chan = takeNextArg(args);
	std::string topic = args; //get the topic

	if (topic[0] == ':')
		topic.erase(0, 1);
	channel = server->getChannel(chan);

	if (!server->findChannel(chan)) //checking if the channel exists
		return server->ft_send(client->getFd(), ERR_NOSUCHCHANNEL(client, chan));
	if (!server->getClient(client->getFd()))
		server->ft_send(client->getFd(), ERR_NOTONCHANNEL(chan));
 	if (topic.empty()) {
		if(channel->getTopic().empty())
			server->ft_send(client->getFd(), RPL_NOTOPIC(client, channel));
		else
			server->ft_send(client->getFd(), RPL_TOPIC(client, channel));
	}
	else {
		if (channel->getMode().find('t') != NPOS && !channel->isOp(client->getFd()))
			return server->ft_send(client->getFd(), ERR_CHANOPRIVSNEEDED(client, channel));
		else {
			channel->setTopic(topic);
			channel->sendChan(NULL, RPL_TOPIC(client, channel));
		}
	}
}