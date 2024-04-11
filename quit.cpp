#include "Server.hpp"

void quit(Client *client, std::string args) {
	Server *server = client->getServer();
	server->sendToClientsInTouch(client, RPL_QUIT(client, args));
	server->removeFromAllChannels(client);
	server->checkEmptyChannels();
}