#include "hpp.hpp"

// /!\ if client is invited, no need of a pass (+i mode overrides +k mode)

void join(Client *client, std::string args) { // simplistic version /!\ not handling yet : modes, invite, password
	size_t pos, space;
	std::string chan_name, chan_key;
	Channel *chan;
	std::vector<std::string> names, keys;
	Server *server = client->getServer();
//std::cerr << "DEBUG-JOIN : '" << args << "'" << std::endl;
	if ((space = args.find(' ')) != std::string::npos && space < args.size() - 1) {// space in arg list -> comma separated channels are followed by corresponding comma separated keys -> lets put these keys in a vector
		chan_key = args.substr(space + 1, args.size());
		chan_name = args.substr(0, space);
		if (chan_key.find(' ') != std::string::npos)
			return ; // ERR bad format
	}
	else
		chan_name = args;
//std::cerr << "DEBUG-JOIN : '" + chan_name + "' '" + chan_key + "'"  << std::endl;

/*	 /!\ TO VERIF	*/
	while (!chan_name.empty()) { // now we recover the channel names
		if ((pos = chan_name.find_first_of(",")) == std::string::npos) // if there are no ',' or ' ' separator, we take the entire string
			pos = chan_name.size();
		names.push_back(chan_name.substr(0, pos));
		chan_name.erase(0, pos + 1);
	}
	while (!chan_key.empty()) {
		if ((pos = chan_key.find(',')) == std::string::npos)
			pos = chan_key.size();
		keys.push_back(chan_key.substr(0, pos));
		chan_key.erase(0, pos);
	}

//std::cerr << "DEBUG-JOIN : " << names.size() << std::endl;

	for (size_t i = 0; i < names.size(); i++) {
		if ((chan = server->getChannel(names[i]))) { // if channel already exists, add the client to its client list
//std::cerr << "DEBUG-JOIN : existing channel '" + names[i] + "'"  << std::endl;
			if (i < keys.size())
				chan->addClient(client, keys[i]);
			else 
				chan->addClient(client, chan_key);
		}
		else { // else we have to create the channel first
//std::cerr << "DEBUG-JOIN : new-channel '" + names[i] + "'"  << std::endl;

			if (i < keys.size()) {
		 		chan = server->addChannel(names[i], keys[i]);
			 	server->getChannel(names[i])->addClient(client, keys[i]);
				chan->giveOp(client->getFd());
			}
			else {
				chan = server->addChannel(names[i]);
			 	server->getChannel(names[i])->addClient(client, chan_key); // NULL -> empty string -> correct ? 
				chan->giveOp(client->getFd());
			}
		}
	}
}