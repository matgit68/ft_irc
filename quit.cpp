#include "Server.hpp"

void quit(Client *client, std::string args) {
	sendToClientsInTouch(client, RPL_QUIT(client, args));
	client->getServer()->checkEmptyChannels();
}

// void quit(Client *client, std::string args) {
//     Server *server = client->getServer();
//     std::string reason = parseReason(args);
//     std::string nick = client->getNick();

//     std::map<std::string, Channel*> channels = server->getChannelMap();

//     // Iterate through each channel
//     std::map<std::string, Channel*>::iterator it;
//     for (it = channels.begin(); it != channels.end(); ++it) {
//         Channel* channel = it->second;

//         // Check if the client is in this channel
//         if (server->getClient(client->getFd())) {
//             std::set<int> clientIds = channel->getClientList();

//             // Iterate through clients in the channel and send quit message
//             std::set<int>::iterator it2;
//             for (it2 = clientIds.begin(); it2 != clientIds.end(); ++it2) {
// 			    Client* destClient = server->getClient(*it2);
//                 if (destClient != NULL) {
//                     ft_send(destClient->getFd(), RPL_QUIT(client, reason));
//                 }
//             }
//         }
//     }
// }


// void quit(Client *client, std::string args) {
// 	Server *server = client->getServer();
// 	std::string reason = parseReason(args);
// 	std::string nick = client->getNick();

// //this loop doesnt work it sends 2 times to itself the msg, we need it to be in channel the msg
// 	std::map<std::string, Channel*> chan = client->getServer()->getChannelMap();
// 	std::set<int>	dest;
// 	for (std::map<std::string, Channel*>::iterator it = chan.begin(); it != chan.end(); it++)
// 	{
// 		std::set<int> tmp = it->second->getClientList();
// 		dest.insert(tmp.begin(), tmp.end());
// 	}
	
// 	for (std::set<int>::iterator it = dest.begin(); it != dest.end(); it++)
// 	{
// 		ft_send(client->getFd(), RPL_QUIT(client, reason));
// 	}	 
	
// }