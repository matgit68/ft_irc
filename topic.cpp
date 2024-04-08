#include "Server.hpp"

void topic(Client *client, std::string args) {

	Server *server = client->getServer();
	Channel *channel;
	std::string chan = takeNextArg(args);
	std::string topic = takeNextArg(args); // get the topic
	if (topic[0] == ':')
		topic.erase(0, 1);
	channel = server->getChannel(chan);

	if (!server->findChannel(chan)) // checking if the channel exists
		return ft_send(client->getFd(), ERR_NOSUCHCHANNEL(chan));

	if (channel->getClientList().find(client->getFd()) == channel->getClientList().end()) // checking if client is on the channel
		return ft_send(client->getFd(), ERR_NOTONCHANNEL(chan));

	if (channel->getMode().find('t') != NPOS && !channel->isOp(client->getFd())) // checking mode and privileges
		return ft_send(client->getFd(), ERR_CHANOPRIVSNEEDED(client, channel));
	else {
		channel->setTopic(topic);
		channel->sendChan(NULL, RPL_TOPIC(client, channel));
	}
}