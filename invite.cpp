#include "Server.hpp"

void list_invite(Server *serv, Client *client, int fd)
{
	std::map<std::string, Channel*> chan = serv->getChannelMap();
	for (std::map<std::string, Channel*>::iterator it = chan.begin();
			it != chan.end(); it++)
	{
		if (it->second->isInvited(fd))
			serv->ft_send(fd, RPL_INVITELIST(client, it->second->getName()));
			/*
				Some rare implementations use numerics 346/347 instead of 336/337 as 
				`RPL_INVITELIST`/`RPL_ENDOFINVITELIST`. You should check the server 
				you are using implements them as expected.

				346/347 now generally stands for `RPL_INVEXLIST`/`RPL_ENDOFINVEXLIST`, 
				used for invite-exception list.
			*/
	}
	serv->ft_send(fd, RPL_ENDOFINVITELIST(client));
}

void invite(Client *client, std::string args) 
{
	Server *server = client->getServer();
	if (args.empty()) 
		return list_invite(client->getServer(), client, client->getFd());
	size_t pos = args.find_first_of(" ");
	if (pos == std::string::npos)
	{
		server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("INVITE"));
		return ;
	}
	std::string	user = args.substr(0, pos);
	args.erase(0, pos + 1);
	if (args.empty()) 
	{
		server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("INVITE"));
		return ;
	}

	Channel	*chan = client->getServer()->getChannel(args);
	if (!chan || args[0] != '#')
	{
		server->ft_send(client->getFd(), ERR_NOSUCHCHANNEL(client, args));
		return;
	}
	if ((chan->getMode().find_first_of("i") != std::string::npos 
				&& !chan->isOp(client->getFd()))
			|| (chan->getMode().find_first_of("i") == std::string::npos
				&& !chan->isClient(client)))
	{
		server->ft_send(client->getFd(), ERR_NOTONCHANNEL(args));
		return;
	}
	if(chan->isClient(client->getServer()->getClient(user)))
	{
		server->ft_send(client->getFd(), ERR_USERONCHANNEL(user, args));
		return;
	}
	int target = client->getServer()->getClient(user)->getFd();
	chan->addInvite(target);

	server->ft_send(client->getFd(), RPL_INVITING(client->getPrefix(), user, args));
	server->ft_send(target, RPL_INVITE(client->getPrefix(), user, args));

}