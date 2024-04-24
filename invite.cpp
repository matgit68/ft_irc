#include "Server.hpp"

static void list_invite(Server *serv, Client *client) {
	std::map<std::string, Channel*> chans = serv->getChannelMap();
	for (std::map<std::string, Channel*>::iterator it = chans.begin(); it != chans.end(); it++)
		if (it->second->isInvited(client->getFd()))
			serv->ft_send(client->getFd(), RPL_INVITELIST(client, it->second->getName()));
	serv->ft_send(client->getFd(), RPL_ENDOFINVITELIST(client));
}

void invite(Client *client, std::string args) {
	Server *server = client->getServer();

	if (args.empty()) // censored by irssi, have to test with "/quote INVITE"
		return list_invite(server, client);

	std::string	user = takeNextArg(args);
	if (user.empty() || args.empty())
		return server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("INVITE"));
	Client *target = server->getClient(user);
	if (!target)
		return server->ft_send(client->getFd(), ERR_NOSUCHNICK(client, user));

	Channel	*chan = server->getChannel(args);
	if (!chan)
		return server->ft_send(client->getFd(), ERR_NOSUCHCHANNEL(client, args));

	if (chan->getMode().find('i') != NPOS && !chan->isOp(client->getFd()))
		return server->ft_send(client->getFd(), ERR_CHANOPRIVSNEEDED(client, chan));
		
	if (!chan->isClient(client))
		return server->ft_send(client->getFd(), ERR_NOTONCHANNEL(args));

	if(chan->isClient(target))
		return server->ft_send(client->getFd(), ERR_USERONCHANNEL(target->getNick(), chan->getName()));

	chan->addInvite(target->getFd());

	server->ft_send(client->getFd(), RPL_INVITING(client->getPrefix(), target->getNick(), chan->getName()));
	server->ft_send(target->getFd(), RPL_INVITE(client->getPrefix(), target->getNick(), chan->getName()));
}