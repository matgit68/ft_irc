#include "Server.hpp"

void quit(Client *client, std::string args) {
	Server *server = client->getServer();
	if(args[0] == ':')
		args.erase(0, 1);
	
	server->delClient(client->getFd(), args);
}