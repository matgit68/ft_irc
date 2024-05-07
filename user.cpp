#include "hpp.hpp"

void user(Client *client, std::string args) {
	Server *server = client->getServer();

	if (client->getStatus()) // can register only once
		return server->ft_send(client->getFd(), ERR_ALREADYREGISTERED(client));

	std::string tmpUser = takeNextArg(args);
	std::string tmp1 = takeNextArg(args);
	std::string tmp2 = takeNextArg(args);
	std::string tmpReal = args;
	if (tmpReal[0] == ':')
		tmpReal.erase(0,1);

	if (tmpUser.empty() || tmp1.empty() || tmp2.empty() || tmpReal.empty())
		return server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("USER"));

	client->setReal(tmpReal);
	client->setUser(tmpUser);

	if (!client->getResponse() && client->getStatus())
		client->getServer()->sendRegistration(client);		
}