#include "hpp.hpp"

void join(Client *client, std::string args) { // simplistic version /!\ not handling yet : modes, invite, password
	size_t pos, space;
	std::string arg;
	Channel *chan;
	std::vector<std::string> names, keys;
	Server *server = client->getServer();

	/!\ FAUX while (!args.empty()) {// now we recover the channel names
		if ((pos = args.find_first_of(", ")) == std::string::npos) // if there are no ',' or ' ' separator, we take the entire string
			pos = args.size();
		names.push_back(args.substr(0, pos));
		args.erase(0, pos + 1);
	}
	if ((space = args.find(' ')) != std::string::npos && space < args.size() - 1) {// space in arg list -> comma separated channels are followed by corresponding comma separated keys -> lets put these keys in a vector
		arg = args.substr(space + 1, args.size());
		while (!arg.empty()) {
			if ((pos = arg.find(',')) == std::string::npos)
				pos = arg.size();
			keys.push_back(arg.substr(0, pos));
			arg.erase(0, pos);
		}
	}
	(void) chan;
	(void) server;
	// if ((chan = client->getServer()->getChannel(channelName))) { // if channel already exists, add the client to its client list
	// 	chan->addClient(client);
	// }
	// else { // else we have to create the channel first
	// 	client->getServer()->addChannel(channelName);
	// 	client->getServer()->getChannel(channelName)->addClient(client);
	// }
}