#include "Server.hpp"

void cap(Client *client, std::string args) {
	if (args[0] == 'L' && args[1] == 'S' && (!args[2] || args[2] == ' '))
		ft_send(client, "CAP * LS :\r\n");
	if (args == "END")
		ft_send(client, "CAP END\r\n");
}