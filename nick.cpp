#include "Server.hpp"

void nick(Client *client, std::string args) {
	if (args[0] == '#' || args[0] == '$' || args[0] == ':'
			|| args.find_first_of(" ,*?!@.") != std::string::npos)
		return ft_send(client, ERR_ERRONEUSNICKNAME(client->getNick(), args));
	
	
	std::string oldNick = client->getNick();
	std::string newNick = args;
	
	if(newNick.length() == 0){
		ft_send(client, ERR_NONICKNAMEGIVEN(client->getNick()));
		return;
	}
	if(!client->getServer()->isNickAvailable(newNick)) //checking if there is already that nick
	{
			ft_send(client, ERR_NICKNAMEINUSE(client->getNick(), newNick));
			return ;
	} 
	if(!is_valid(newNick)) //is_valid() func is in the utils.cpp
	{
		ft_send(client, ERR_ERRONEUSNICKNAME(client->getNick(), newNick));
		return;
	}
	if(oldNick.empty()) {  //changin the oldNick with the newNick
		client->setNick(newNick);
		std::cout << "Requesting the new nick \"" <<  newNick << "\"." << std::endl;
	}else {
		client->setOldNick(oldNick);
		client->setNick(newNick);
		std::cout << client->getOldNick() << " changed his nickname to " << newNick << "." << std::endl;
	}
}