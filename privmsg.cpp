#include "Server.hpp"

void privmsg(Client *client, std::string args) {
	Server *server = client->getServer();
	std::string	destlist = args.substr(0, args.find_first_of(" "));
	std::vector<std::string>	dest;

	for (size_t pos = 0; !destlist.empty();) {// decompose the DEST list
		if ((pos = destlist.find_first_of(",")) == NPOS) // if there are no ',' or ' ' separator, we take the entire string
			pos = destlist.size();
		dest.push_back(destlist.substr(0, pos));
		destlist.erase(0, pos + 1);
	}

	if (dest.empty())
		return server->ft_send(client->getFd(), ERR_NORECIPIENT(client));

	args.erase(0, args.find_first_of(" ") + 1);
	if (args.empty())
		return server->ft_send(client->getFd(), ERR_NOTEXTTOSEND(client));
	if (args[0] != ':')
		args = ":" + args;	// easy syntax from NC but not totaly correct, to discuss
	for (size_t i = 0; i < dest.size(); i++) {
		if ((dest[i])[0] == '&' || (dest[i])[0] == '#') {// CHANNEL
			Channel* chan;
			if ((chan = server->getChannel(dest[i]))) {
				if (!chan->getPasswd().empty() && !chan->isClient(client)) // user's right
					server->ft_send(client->getFd(), ERR_CANNOTSENDTOCHAN(client, dest[i]));
				else
					chan->sendChan(client, RPL_PRIVMSG(client, chan, args));
			}
			else // chan doesn't exist
				server->ft_send(client->getFd(), ERR_NOSUCHNICK(client, dest[i]));
		}
		else // USER
		{
			Client* target;
			if ((target = server->getClient(dest[i])))
				server->ft_send(target->getFd(), ":" + client->getNick() + " PRIVMSG " + dest[i] + " " + args + "\r\n");
			else // ERR msg RPL_AWAY ? ERR_NOSUCHNICK ? 
				server->ft_send(client->getFd(), ERR_NOSUCHNICK(client, dest[i])); // If <target> is a user and that user has been set as away, the server may reply with an RPL_AWAY (301) numeric and the command will continue.
		}
	}
}