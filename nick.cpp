#include "Server.hpp"

void nick(Client *client, std::string args) {
	
	std::string oldNick = client->getNick();
	std::string newNick = args;
	Server *server = client->getServer();

	if (client->getPasswd() == false)
		return;
	if(newNick.length() == 0)
		return ft_send(client->getFd(), ERR_NONICKNAMEGIVEN(client));
	if(!client->getServer()->isNickAvailable(newNick)) //checking if there is already that nick
		return ft_send(client->getFd(), ERR_NICKNAMEINUSE(client, newNick));
	if(!is_valid(newNick)) 
		return ft_send(client->getFd(), ERR_ERRONEUSNICKNAME(client, newNick));

	if(oldNick.empty())
		client->setNick(newNick);
	else {
		client->setOldNick(oldNick);
		client->setNick(newNick);
	}
	server->sendToClientsInTouch(client, RPL_NICK(client));
}
