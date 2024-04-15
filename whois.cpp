#include "hpp.hpp"

void	whois(Client *client, std::string args) {
	if (args.empty())
		return 	ft_send(client->getFd(), RPL_WHOISUSER(client, client));
//		return ft_send(client->getFd(), ERR_NEEDMOREPARAMS("USER"));
	size_t pos = args.find(' ');
	if (pos != NPOS)
		args.substr(0, pos);
	Client *target = client->getServer()->getClient(args);
	if (!target)
		return ft_send(client->getFd(), ERR_NOSUCHNICK(client, args));
//	RPL_WHOISUSER
//	"<client> <nick> <username> <host> * :<realname>"
	ft_send(client->getFd(), RPL_WHOISUSER(client, target));
}