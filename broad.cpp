#include "Server.hpp"

void broad(Client* client, std::string msg) {
	client->getServer()->broadcast(client, msg);
}