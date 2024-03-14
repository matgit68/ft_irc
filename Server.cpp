#include "Server.hpp"

Server::Server(int p, std::string pass): _port(p), _passwd(pass) {
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
}
