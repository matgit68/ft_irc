#include "Server.hpp"

void nick(Client *client, std::string args) {
	if (args[0] == '#')
		return ft_send(client, "ERR_ERRONEUSNICKNAME");
	client->setNick(args);
}