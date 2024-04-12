#include "Server.hpp"

void nick(Client *client, std::string args) {
	
	std::string oldNick = client->getNick();
	std::string newNick = args;
	Server *server = client->getServer();

	if (client->getPasswd() == false)
		return;
	if(newNick.length() == 0)
		return server->ft_send(client->getFd(), ERR_NONICKNAMEGIVEN(client));
	if(!is_valid(newNick))
		return server->ft_send(client->getFd(), ERR_ERRONEUSNICKNAME(client, newNick));
	if(!client->getServer()->isNickAvailable(newNick)) {
		if(!client->getNick().empty())
			return server->ft_send(client->getFd(), ERR_NICKNAMEINUSE(client, newNick));
		else {
			while(!client->getServer()->isNickAvailable(newNick))
				newNick.append("_");
			client->setOldNick(args);
			client->setNick(newNick);
			server->ft_send(client->getFd(), RPL_NICK(client));
		}
	}
	else {
		if(oldNick.empty())
			client->setNick(newNick);
		else {
			client->setOldNick(oldNick);
			client->setNick(newNick);
		}
		server->sendToClientsInTouch(client, RPL_NICK(client), true);
	}
}
