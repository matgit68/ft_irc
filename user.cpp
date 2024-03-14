#include "Server.hpp"

// Command: USER
//   Parameters: <username> 0 * <realname>
	// for me '0' is the hostname
	//		'*' is servername
	// i don't remerber where i've seen thqt, i don't find it anymore
	// have you see something like this to fill ur blank ?
// USER user_name user_name 127.0.0.1 :real_name

void user(Client *client, std::string args) {
	if (client->getStatus()) // is client already registered ?
		return ft_send(client, "ERR_ALREADYREGISTERED\r\n");
	size_t pos = args.find(' ');
	if (pos == std::string::npos)
		return ft_send(client, "ERR_NEEDMOREPARAMS\r\n");

	std::string tmpUser = args.substr(0, pos); //get user_name
	args.erase(0, pos + 1);

	pos = args.find(' '); // double ? seems that irssi does not respect protocol specs
	if (pos == std::string::npos)	// before, I erased all spaces, so i havn't the pb --> to your choice
		return ft_send(client, "ERR_NEEDMOREPARAMS\r\n");

	if (tmpUser != args.substr(0, pos))
		std::cout << "user_name is not user_same" << std::endl;
	args.erase(0, pos + 1);

	pos = args.find(' '); // now we go for real_name

	std::string tmpReal = args.substr(pos +1, args.size());
	if (tmpReal[0] == ':')
		tmpReal.erase(0, 1);
	client->setReal(tmpReal);
	client->setUser(tmpUser);
//	std::cout << "UserName: _" << client->getUser() << "_ RealName: _" << client->getReal() << '_' << std::endl;
	client->getServer()->sendRegistration(client);
		// are you sure that you always reveive the USER request like the last one ? this was the reason of the _response bool
		// -> always w/ irssi client ¯\_(ツ)_/¯/quit
		
}