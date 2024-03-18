#include "Server.hpp"

void nick(Client *client, std::string args) {
	if (args[0] == '#' || args[0] == '$' || args[0] == ':'
			|| args.find_first_of(" ,*?!@.") != std::string::npos)
		return ft_send(client->getFd(), ERR_ERRONEUSNICKNAME(client->getNick(), args));
	
	
	std::string oldNick = client->getNick();
	std::string newNick = args;
	
	if(newNick.length() == 0){
		ft_send(client->getFd(), ERR_NONICKNAMEGIVEN(client->getNick()));
		return;
	}
	if(!client->getServer()->isNickAvailable(newNick)) //checking if there is already that nick
	{
			ft_send(client->getFd(), ERR_NICKNAMEINUSE(client->getNick(), newNick));
			return ;
	} 
	if(!is_valid(newNick)) //is_valid() func is in the utils.cpp
	{
		ft_send(client->getFd(), ERR_ERRONEUSNICKNAME(client->getNick(), newNick));
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
	

// Nicknames are non-empty strings with the following restrictions:

//     They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
//     They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
//     They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
//     They SHOULD NOT contain any dot character ('.', 0x2E).