#include "Server.hpp"

Server::Server(int p, std::string pass): _port(p), _passwd(pass), _hostname("localhost"), _createdTime(timestring()) {
	initFunPtr();
}

Server::~Server() {}

Server::Server(Server const &ref) {
	*this = ref;
}

Server &Server::operator=(Server const &ref) {
	_fd = ref._fd;
	_port = ref._port;
	_passwd = ref._passwd;
	_clients = ref._clients;
	_channels = ref._channels;
	return *this;
}

int Server::getFd() const { return _fd; }

int Server::getPort() const { return _port; }

std::string Server::getPasswd() const { return _passwd; }



Client *Server::getClient(int fd) {
	if (_clients.find(fd) == _clients.end())
		return NULL;
	return _clients[fd];
}

Channel *Server::getChannel(std::string name) {
	if (_channels.find(name) == _channels.end())
		return NULL;
	return _channels[name];
}

std::string Server::getCreatedTime(void) const { return _createdTime; }

funPtr Server::getCommand(std::string name) {
	if (_commands.find(name) == _commands.end())
		return NULL;
return _commands[name];
}

void Server::initFunPtr() {
	_commands["PING"] = &ping;
	_commands["KICK"] = &kick;
	_commands["INVITE"] = &invite;
	_commands["TOPIC"] = &topic;
	_commands["MODE"] = &mode;
	_commands["CAP"] = &cap;
	_commands["PASS"] = &pass;
	_commands["NICK"] = &nick;
	_commands["USER"] = &user;
	_commands["QUIT"] = &quit;
	_commands["JOIN"] = &join;
	_commands["PRIVMSG"] = &privmsg;
	_commands["BROAD"] = &broad;
}

void Server::sendRegistration(Client *client) {
	ft_send(client, ":localhost 001 " + client->getNick() + " :Welcome to the 42Mulhouse Network, " + client->getPrefix() + "\r\n");
	ft_send(client, ":localhost 002 " + client->getNick() + " :Your host is "+ _hostname + ", running version WTF\r\n");
	ft_send(client, ":localhost 003 " + client->getNick() + " :This server was created " + _createdTime + "\r\n");
//	 "004 <client> <servername> <version> <available user modes> <available channel modes> [<channel modes with a parameter>]"
	client->setResponse(); // if we need to confirm the protocol
}

void Server::broadcast(Client* client, std::string msg) {
	std::map<int, Client*>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); it++)
		if (it->second != client)
			ft_send(it->second, msg);
}

bool Server::isNickAvailable(std::string& newNick)
{
	std::map<int, Client*>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); it++)
	{
		if(it->second->getNick() == newNick)
			return false;
	}
	return true;
}

