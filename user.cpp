#include "Server.hpp"

// Command: USER
//   Parameters: <username> 0 * <realname>

void user(Client *client, std::string args) {
	if (client->getStatus()) {
		send(client->getFd(), "ERR_ALREADYREGISTERED\r\n", 23, 0);
		return;
	}
	size_t pos = args.find(' ');
	if (pos == std::string::npos) {
		send(client->getFd(), "ERR_NEEDMOREPARAMS\r\n", 20, 0);
		return;
	}
	std::string tmpUser = args.substr(0, pos);
	if (args.substr(pos, 6) != " 0 * :") {
		send(client->getFd(), "ERR_NEEDMOREPARAMS\r\n", 20, 0);
		return;
	}
	client->setUser(tmpUser);
	client->setReal(args.substr(pos + 6, args.size()));
}