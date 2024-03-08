#include "Server.hpp"

Server::Server(int p, std::string pass): port(p), passwd(pass) {
}

Server::~Server() {}

Server::Server(Server const &ref) {
	*this = ref;
}

Server &Server::operator=(Server const &ref) {
	fd = ref.fd;
	port = ref.port;
	passwd = ref.passwd;
	clients = ref.clients;
	channels = ref.channels;
	return *this;
}

int Server::getFd() const { return fd; }

int Server::getPort() const { return port; }

Client *Server::getClient(int fd) {
	if (clients.find(fd) == clients.end())
		return NULL;
	return clients[fd];
}

Channel *Server::getChannel(std::string name) {
	if (channels.find(name) == channels.end())
		return NULL;
	return channels[name];
}

