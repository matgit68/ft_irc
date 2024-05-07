#include "Server.hpp"

void pass(Client *client, std::string args) {
	if (client->getResponse())
		return client->getServer()->ft_send(client->getFd(), ERR_ALREADYREGISTERED(client));
	if (args != client->getServer()->getPasswd())
		return client->getServer()->ft_send(client->getFd(), ERR_PASSWDMISMATCH(client));
	client->setPasswd(true);
}