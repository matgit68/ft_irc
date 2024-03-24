#include "Server.hpp"

void list_invite(Server *serv, Client *client, int fd)
{
	std::map<std::string, Channel*> chan = serv->getChannelMap();
	for (std::map<std::string, Channel*>::iterator it = chan.begin();
			it != chan.end(); it++)
	{
		if (it->second->isInvite(fd))
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
	(void)args;
	(void)client;

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


	// std::string userNick = client->getNick();
	
	// std::string channel = args.getParam(0);
	// std::string target = args.getParam(1);

	// if(channel[0] != '#'){
	// 	sendClient(client->getFd(), ERR_NOSUCHCHANNEL(userNick, channel));
	// 	return;
	// }
	// if(!_channels.find(channel) == _channels.end()){
	// 	send(client->getFd(), "ERR_NOSUCHCHANNEL\r\n", 19, 0);
	// 	return;
	// }
	// if(!_channels[channel]->isOp(userNick)){
	// 	send(client->getFd(), "ERR_NOTONCHANNEL\r\n", 18, 0);
	// 	return;
	// }
	// if(_channels[channel]->isOp(target)){
	// 	send(client->getFd(), "ERR_USERONCHANNEL\r\n", 19, 0);
	// 	return;
	// }
}