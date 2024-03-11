#include "Client.hpp"

Client::Client(int f, Server *s): _server(s), _fd(f), _clientReady(false), _response(false) {}

Client::~Client() {}

int Client::getFd() const { return _fd; }

std::string Client::getUser() const { return _user; }

std::string Client::getNick() const { return _nick; }

void Client::setUser(std::string u) { _user = u; }

void Client::setNick(std::string n) { _nick = n; }

// void Client::receive(char* str) {
// 	_buffer.append(str);
// 	size_t pos = _buffer.find("\r\n");
// 	if (pos != std::string::npos) {
// 		parse(_buffer.substr(0, pos + 2));
// 		_buffer.erase(0, pos + 2);
// 	}
// }

// purpose of receive v2
// we used substr() instead of clear() in case there are serveral CRLF in the same buffer, so we should cycle the buffer with while() until there's no CRLF left ?
// or what ??
void Client::receive(char* str) { 
	_buffer.append(str);
	size_t pos;
	while ((pos= _buffer.find("\r\n")) != std::string::npos) {
		parse(_buffer.substr(0, pos));
		_buffer.erase(0, pos + 2);
	}
}

void Client::parse(std::string msg) {
	size_t pos;
	if ((pos = msg.find_first_of(' ')) == std::string::npos) {
		_server->broadcast(this, msg); // for testing purposes
		// send an error ERR_NEEDMOREPARAMS (461) ?
		return ;
	}
	funPtr f = _server->getCommand(msg.substr(0, pos));
	if (f != NULL)
		f(this, msg.substr(pos + 1, msg.size()));
	else
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

bool Client::getResponse() const { return _response; }

void Client::setStatus(void) { _clientReady = !_clientReady; }

void Client::setResponse(void) {
	_response = !_response;
	if (_response)// choose ur message
	{
// 		std::string end = "001 " + _user + " " + _nick + " :Welcome to the Internet Relay Network " + _nick + "\r\n";
// std::cerr << "debug CAP END : " + end;
// 		if (send(_fd, end.c_str(), end.size(), 0) != (ssize_t) end.size())
// 			std::cerr << "Error sending msg" << std::endl;
		std::string wel = "001 " + _nick + " :Welcome to the <networkname> Network, " + this->getPrefix() + "\r\n";
		std::cerr << "debug CAP END : " + wel;
		if (send(_fd, wel.c_str(), wel.size(), 0) != (ssize_t) wel.size())
			std::cerr << "Error sending msg" << std::endl;
	}
}
void Client::setHost(std::string h) { _host = h; }
void Client::setReal(std::string r) { _real = r; }
