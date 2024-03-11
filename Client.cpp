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

bool Client::getStatus(void) const { return _clientReady; }
std::string Client::getHost() const { return _host; }
std::string Client::getPrefix() const { 
	return (_nick.empty() ? "*" \
		: (_nick + (_user.empty() ? "" : "!" + _user) +
		(_host.empty() ? "" : "@" + _host)));
	}
std::string Client::getReal() const { return _real; }

void Client::setStatus(void) { _clientReady = !_clientReady; }
void Client::setHost(std::string h) { _host = h; }
void Client::setReal(std::string r) { _real = r; }
