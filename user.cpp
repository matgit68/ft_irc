#include "hpp.hpp"

// Command: USER
//   Parameters: <username> 0 * <realname>
	// for me '0' is the hostname
	//		'*' is servername
	// i don't remerber where i've seen thqt, i don't find it anymore
	// have you see something like this to fill ur blank ?
// USER user_name user_name 127.0.0.1 :real_name

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