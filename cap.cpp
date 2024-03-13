#include "Server.hpp"

void cap(Client *client, std::string args) {
	if (args == "END")
		ft_send(client, "CAP END\r\n");
}