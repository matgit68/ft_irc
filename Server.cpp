#include "Server.hpp"

Server::Server(int p, std::string pass): _port(p), _passwd(pass) {
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

