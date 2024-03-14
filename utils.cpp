#include <string>
#include <sys/types.h>
#include <sys/socket.h>

void ft_send(Client *client, std::string msg) {
	std::cout << "Sent(fd" << client->getFd() << ") : " << msg << std::endl;
	send(client->getFd(), msg.c_str(), msg.size(), 0);
}
