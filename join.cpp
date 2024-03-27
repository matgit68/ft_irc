#include "hpp.hpp"

void join(Client *client, std::string args) {
	std::string chan_name, chan_key;
	Channel *chan;
	std::vector<std::string> names, keys;
	Server *server = client->getServer();

	chan_name = takeNextArg(args);
	if (!args.empty())
		chan_key = takeNextArg(args);
	while (!chan_name.empty()) // now we recover the channel names
		names.push_back(takeNextArg(',', chan_name));
	while (!chan_key.empty())
		keys.push_back(takeNextArg(',', chan_key));

	for (size_t i = 0; i < names.size(); i++) {
		if ((!(chan = server->getChannel(names[i])))) // Channel doesn't exist
			server->createChannel(names[i], client);
		else {
			if (chan->getMode().find('i') != NPOS) // invite mode is set. invite overrides +l and +k
				chan->addClientInvite(client);
			else if (chan->getMode().find('k') != NPOS) { // key mode is set
			 	if (keys.empty())
					ft_send(client->getFd(), ERR_BADCHANNELKEY(client, chan)); // no password was given
				else {
					chan->addClientPass(client, keys.front()); // try to join with first key
					keys.erase(keys.begin()); // delete used key
				}
			}
			else
				chan->addClient(client);
		}
	}
}