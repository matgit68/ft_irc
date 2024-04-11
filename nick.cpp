#include "Server.hpp"

void nick(Client *client, std::string args) {
	
	std::string oldNick = client->getNick();
	std::string newNick = args;
	Server *server = client->getServer();

	if (client->getPasswd() == false)
		return;
	if(newNick.length() == 0)
		return ft_send(client->getFd(), ERR_NONICKNAMEGIVEN(client));
	if(!is_valid(newNick))
		return ft_send(client->getFd(), ERR_ERRONEUSNICKNAME(client, newNick));
	if(!client->getServer()->isNickAvailable(newNick)) {
		if(!client->getNick().empty())
			return ft_send(client->getFd(), ERR_NICKNAMEINUSE(client, newNick));
		else {
			while(!client->getServer()->isNickAvailable(newNick))
				newNick.append("_");
			std::cout << "alternative nick-> " << newNick << std::endl;
			client->setOldNick(args);
			client->setNick(newNick);
			ft_send(client->getFd(), RPL_NICK(client));
		}
	}
	else {
		if(oldNick.empty())
			client->setNick(newNick);
		else {
			client->setOldNick(oldNick);
			client->setNick(newNick);
		}
		server->sendToClientsInTouch(client, RPL_NICK(client), false);
	}
}
