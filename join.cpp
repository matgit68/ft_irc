#include "hpp.hpp"

static void prefixChan(std::string &name) {
	if (name[0] != '#')
		name.insert(name.begin(), '#');
}

void join(Client *client, std::string args) {
	std::string chan_name, chan_key;
	std::string tmp;
	Channel *chan;
	std::vector<std::string> names, keys;
	Server *server = client->getServer();

	if (args.empty())
		return server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("JOIN"));
	args = toLowercase(args);
	chan_name = takeNextArg(args);
	if (!args.empty())
		chan_key = takeNextArg(args);
	while (!chan_name.empty()) { // recovering channel names
		prefixChan(chan_name);
		names.push_back(takeNextArg(',', chan_name));
	}
	while (!chan_key.empty()) // recovering keys
		keys.push_back(takeNextArg(',', chan_key));

	for (size_t i = 0; i < names.size(); i++) {
		if ((!(chan = server->getChannel(names[i])))) // Channel doesn't exist
			server->createChannel(names[i], client);
		else {
			if (chan->getMode().find('i') != NPOS) // invite mode is set. invite overrides +l and +k
				chan->addClientInvite(client);
			else if (chan->getMode().find('k') != NPOS) { // key mode is set
			 	if (keys.size() < i)
					server->ft_send(client->getFd(), ERR_BADCHANNELKEY(client, chan)); // no password was given
				else
					chan->addClientPass(client, keys[i]); // try to join with first key
			}
			else
				chan->addClient(client);
		}
	}
}