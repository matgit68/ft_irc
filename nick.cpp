#include "Server.hpp"

void nick(Client *client, std::string args) {
	if (args[0] == '#' || args[0] == '$' || args[0] == ':'
			|| args.find_first_of(" ,*?!@.") != std::string::npos)
		return ft_send(client, ERR_ERRONEUSNICKNAME(client->getNick(), args));
	client->setNick(args);


	std::string newNick = args;
	std::string oldNick = client->getNick();
	
	std::cout << "new nickname: " << newNick << std::endl;
	std::cout << "old nickname: " << oldNick << std::endl;

	if(newNick.length() == 0){
		ft_send(client, ERR_NONICKNAMEGIVEN(client->getNick()));
		return;
	}
	if(client->getServer()->isNickAvailable(newNick)) // I cant check if its working it can be client->getNick() also
	{
			ft_send(client, ERR_NICKNAMEINUSE(client->getNick(), newNick));
			return ;
	} 
	if(!is_valid(newNick))
	{
		ft_send(client, ERR_ERRONEUSNICKNAME(client->getNick(), newNick));
		return;
	}
	client->setNick(newNick);
}
	

// Nicknames are non-empty strings with the following restrictions:

//     They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
//     They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
//     They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
//     They SHOULD NOT contain any dot character ('.', 0x2E).