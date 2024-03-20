#include "Server.hpp"

void privmsg(Client *client, std::string args) {
	(void) client;
	(void) args;

	std::string	destlist = args.substr(0, args.find_first_of(" "));
	std::vector<std::string>	dest;

	for (size_t pos = 0; !destlist.empty();) {// decompose the DEST list
		if ((pos = destlist.find_first_of(",")) == NPOS) // if there are no ',' or ' ' separator, we take the entire string
			pos = destlist.size();
		dest.push_back(destlist.substr(0, pos));
		destlist.erase(0, pos + 1);
	}
	if (dest.empty())
	{
		ft_send(client->getFd(), ERR_NORECIPIENT(client->getUser()));
		return ;
	}
	args.erase(0, args.find_first_of(" ") + 1);
	if (args.empty())
	{
		ft_send(client->getFd(), ERR_NOTEXTTOSEND(client->getUser()));
		return ;
	}
	if (args[0] != ':')
		args = ":" + args;	// easy syntax from NC but not totaly correct, to discuss
	for (size_t i = 0; i < dest.size(); i++) {
		if ((dest[i])[0] == '&' || (dest[i])[0] == '#') // CHANNEL
		{
			Channel* chan;
			if ((chan = client->getServer()->getChannel(dest[i])))
			{
				if (!chan->getPasswd().empty() && !chan->isClient(client)) // user's right
					ft_send(client->getFd(), ERR_CANNOTSENDTOCHAN(client->getUser(), dest[i]));
				else
					chan->sendChan(client, RPL_PRIVMSG(client->getNick(), chan->getName(), args));
			}
			else // chan doesn't exist
				ft_send(client->getFd(), ERR_NOSUCHNICK(client->getUser(), dest[i]));
		}
		else // USER
		{
			Client* target;
			if ((target = client->getServer()->getClient(dest[i])))
				ft_send(target->getFd(), ":" + client->getNick() + " PRIVMSG " + dest[i] + " " + args + "\r\n");
			else // ERR msg RPL_AWAY ? ERR_NOSUCHNICK ? 
				ft_send(client->getFd(), ERR_NOSUCHNICK(client->getNick(), dest[i])); // If <target> is a user and that user has been set as away, the server may reply with an RPL_AWAY (301) numeric and the command will continue.
		}
		/*
		  :Angel PRIVMSG Wiz :Hello are you receiving this message ?
                                  ; Message from Angel to Wiz.

  			:dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!
                                  ; Message from dan to the channel
                                  #coolpeople
		*/
	}
}