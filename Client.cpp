#include "Client.hpp"

Client::Client(int f): fd(f), clientReady(false) {}

Client::~Client() {}

int Client::getFd() const { return fd; }

std::string Client::getUser() const { return user; }

std::string Client::getNick() const { return nick; }

void Client::setUser(std::string u) { user = u; }

void Client::setNick(std::string n) { nick = n; }

void Client::receive(Server* server, char* str) {
	buffer.append(str);
	if (buffer.find("\r\n") != std::string::npos) {
		parse(server);
		buffer.clear();
	}
}

void Client::parse(Server* server) {
	std::cout << "PARSE " << buffer;
	server->broadcast(this, buffer); // for testing purposes
}