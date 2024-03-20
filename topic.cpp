#include "Server.hpp"

// size_t pos = args.find(' ');
// 	if (pos == std::string::npos)
// 		return ft_send(client, "ERR_NEEDMOREPARAMS\r\n");

// 	std::string tmpUser = args.substr(0, pos); //get user_name
// 	args.erase(0, pos + 1);

// 	pos = args.find(' '); // double ? seems that irssi does not respect protocol specs
// 	if (pos == std::string::npos)	// before, I erased all spaces, so i havn't the pb --> to your choice
// 		return ft_send(client, "ERR_NEEDMOREPARAMS\r\n");

// 	if (tmpUser != args.substr(0, pos))
// 		std::cout << "user_name is not user_same" << std::endl;
// 	args.erase(0, pos + 1);

// 	pos = args.find(' '); // now we go for real_name

// 	std::string tmpReal = args.substr(pos +1, args.size());
// 	if (tmpReal[0] == ':')
// 		tmpReal.erase(0, 1);
// 	client->setReal(tmpReal);
// 	client->setUser(tmpUser);
// 	std::cout << "UserName: _" << client->getUser() << "_ RealName: _" << client->getReal() << '_' << std::endl;
// 	client->getServer()->sendRegistration(client);
	

void topic(Client *client, std::string args) {

	Server *server = client->getServer();
	size_t pos = args.find(' ');
	if(pos == std::string::npos)
		return ft_send(client, ERR_NEEDMOREPARAMS(client->getCommand()));
	std::string chan = args.substr(0, pos); //get the channel
	args.erase(0, pos + 1);
	if(chan[0] == '#')
		chan.erase(0, 1);
	else
	 	return ft_send(client, ERR_NOSUCHCHANNEL(chan));
	
	pos = args.find(' ');
	std::string topic = args.substr(pos + 1, args.size()); //get the topic
	if(topic[0] == ':')
		topic.erase(0, 1);
	
	if(!server->findChannel(chan)) //checking if the user is in the channel
	{
		ft_send(client, ERR_NOSUCHCHANNEL(chan));
	}
	
	std::cout << "channel -> " << chan << std::endl;
	std::cout << "topic -> [" << topic << "]" << std::endl;
	if(topic.empty())  // if there are only spaces in the topic, it accepts like that, i looked origina version ot was the same
		ft_send(client, RPL_NOTOPIC(client->getNick(), chan));
	else
		ft_send(client, RPL_TOPIC(client->getNick(), chan, topic));

	// std::vector<std::string> names;
	// for(size_t i = 0; i < names.size(); i++){
	// 	if(Channel *channels = server->getChannel(names[i]))
	// 	std::cout << "existing channel: " << channels << std::endl;

	// std::map<std::string, Channel*>		 channels = server->getChannels();
	// std::map<std::string, Channel*>::iterator channel = channels.find(chan);
	//checking if the user exists
	//is the person operator && mode
	//setTopic()
		
}