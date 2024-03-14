#include "Server.hpp"



void invite(Client *client, std::string args) 
{
	(void)args;
	(void)client;

	

	// std::string userNick = client->getNick();
	
	// if (args.empty()) {
	// 	sendClient(client->getFd(), ERR_NEEDMOREPARAMS(client->getUser(), client->getCommand()));
	// 	return ;
	// }
	
	// std::string channel = args.getParam(0);
	// std::string target = args.getParam(1);

	// if(channel[0] != '#'){
	// 	sendClient(client->getFd(), ERR_NOSUCHCHANNEL(userNick, channel));
	// 	return;
	// }
	// if(!_channels.find(channel) == _channels.end()){
	// 	send(client->getFd(), "ERR_NOSUCHCHANNEL\r\n", 19, 0);
	// 	return;
	// }
	// if(!_channels[channel]->isOp(userNick)){
	// 	send(client->getFd(), "ERR_NOTONCHANNEL\r\n", 18, 0);
	// 	return;
	// }
	// if(_channels[channel]->isOp(target)){
	// 	send(client->getFd(), "ERR_USERONCHANNEL\r\n", 19, 0);
	// 	return;
	// }
}