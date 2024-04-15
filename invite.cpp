#include "Server.hpp"

void list_invite(Server *serv, Client *client, int fd)
{
	std::map<std::string, Channel*> chan = serv->getChannelMap();
	for (std::map<std::string, Channel*>::iterator it = chan.begin();
			it != chan.end(); it++)
	{
		if (it->second->isInvited(fd))
			ft_send(fd, RPL_INVITELIST(client, it->second->getName()));
			/*
				Some rare implementations use numerics 346/347 instead of 336/337 as 
				`RPL_INVITELIST`/`RPL_ENDOFINVITELIST`. You should check the server 
				you are using implements them as expected.

				346/347 now generally stands for `RPL_INVEXLIST`/`RPL_ENDOFINVEXLIST`, 
				used for invite-exception list.
			*/
	}
	ft_send(fd, RPL_ENDOFINVITELIST(client));
}

void invite(Client *client, std::string args) 
{
	if (args.empty()) 
	{
		list_invite(client->getServer(), client, client->getFd());
		return ;
	}
	size_t pos = args.find_first_of(" ");
	if (pos == std::string::npos)
	{
		ft_send(client->getFd(), ERR_NEEDMOREPARAMS("INVITE"));
		return ;
	}
	std::string	user = args.substr(0, pos);
	args.erase(0, pos + 1);
	if (args.empty()) 
	{
		ft_send(client->getFd(), ERR_NEEDMOREPARAMS("INVITE"));
		return ;
	}

	Channel	*chan = client->getServer()->getChannel(args);
	if (!chan || args[0] != '#')
	{
		ft_send(client->getFd(), ERR_NOSUCHCHANNEL(args));
		return;
	}
	if ((chan->getMode().find_first_of("i") != std::string::npos 
				&& !chan->isOp(client->getFd()))
			|| (chan->getMode().find_first_of("i") == std::string::npos
				&& !chan->isClient(client)))
	{
		ft_send(client->getFd(), ERR_NOTONCHANNEL(args));
		return;
	}
	if(chan->isClient(client->getServer()->getClient(user)))
	{
		ft_send(client->getFd(), ERR_USERONCHANNEL(user, args));
		return;
	}
	int target = client->getServer()->getClient(user)->getFd();
	chan->addInvite(target);

	ft_send(client->getFd(), RPL_INVITING(client->getPrefix(), user, args));
	ft_send(target, RPL_INVITE(client->getPrefix(), user, args));

}