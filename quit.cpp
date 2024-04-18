#include "Server.hpp"

void quit(Client *client, std::string args) {
	Server *server = client->getServer();
	if(args[0] == ':')
		args.erase(0, 1);
	
	server->sendToClientsInTouch(client, RPL_QUIT(client, args), false);
	server->removeFromAllChannels(client);
	server->checkEmptyChannels();
	server->delClient(client->getFd());
}