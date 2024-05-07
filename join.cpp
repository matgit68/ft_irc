#include "hpp.hpp"

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
		if ((!(chan = server->getChannel(names[i])))) { // Channel doesn't exist
			server->createChannel(names[i], client);
			continue;
		}
		if (chan->isInvited(client->getFd())) { // client is invited. Invite overrides +l and +k
			chan->addClientInvite(client);
			continue;
		}
		if (chan->getMode().find('i') != NPOS) { // invite mode is set and client was not invited.
			server->ft_send(client->getFd(), ERR_INVITEONLYCHAN(chan));
			continue;
		}
		if (chan->getMode().find('k') != NPOS) { // key mode is set
			if (keys.empty() || keys.size() < i)
				server->ft_send(client->getFd(), ERR_BADCHANNELKEY(client, chan)); // no password was given
			else
				chan->addClientPass(client, keys[i]); // try to join with first key
			continue;
		}
		chan->addClient(client); // no mode, any client can join
	}
}