#include "Client.hpp"

Client::Client(int f, Server *s): _server(s), _fd(f), _clientReady(false) {}

Client::~Client() {}

int Client::getFd() const { return _fd; }

std::string Client::getUser() const { return _user; }

std::string Client::getNick() const { return _nick; }

void Client::setUser(std::string u) { _user = u; }

void Client::setNick(std::string n) { _nick = n; }

void Client::receive(char* str) {
	_buffer.append(str);
	size_t pos = _buffer.find("\r\n");
	if (pos != std::string::npos) {
		parse(_buffer.substr(0, pos + 2));
		_buffer.erase(0, pos + 2);
	}
}

void Client::parse(std::string msg) {
	std::cout << "PARSE " << msg;
	_server->broadcast(this, msg); // for testing purposes
}