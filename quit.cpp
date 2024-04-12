#include "Server.hpp"

void quit(Client *client, std::string args) {
	Server *server = client->getServer();
	if(args[0] == ':')
		args.erase(0, 1);
	
	server->sendToClientsInTouch(client, RPL_QUIT(client, args), false);
	server->removeFromAllChannels(client);
	server->checkEmptyChannels();
	if (close(client->getFd()) == FAIL) // close fd
		std::cerr << "couldnt close fd " << client->getFd() << std::endl;
	else
		std::cout << RED "Client " << client->getFd() << " disconnected" RESET << std::endl;
	client->setGone(true);
}