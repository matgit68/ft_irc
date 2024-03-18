#include "Client.hpp"

Client::Client(int f, Server *s): _host("localhost"), _server(s), _fd(f), _clientReady(false), _response(false), _passwd(false) {}

Client::~Client() {}

int Client::getFd() const { return _fd; }

std::string Client::getUser() const { return _user; }

std::string Client::getNick() const { return _nick; }

std::string Client::getOldNick() const { return _oldnick; }

Server *Client::getServer() const { return _server; }

void Client::setUser(std::string u) { _user = u; }

void Client::setNick(std::string n) { _nick = n; }

void Client::setPasswd(bool b) { _passwd = b; }

void Client::setOldNick(std::string nickname) { _oldnick = nickname; }

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
	if (_buffer.size() > 512 ) { // Servers SHOULD gracefully handle messages over the 512-bytes limit. They may send an error numeric back, preferably ERR_INPUTTOOLONG (417) 
		send(_fd, "ERR_INPUTTOOLONG\r\n", 18, 0);
		_buffer.clear();
	}
	size_t pos;
	while ((pos = _buffer.find("\r\n")) != std::string::npos) {
		parse(_buffer.substr(0, pos));
		_buffer.erase(0, pos + 2);
	}
}

void Client::parse(std::string msg) {
	Client *client = this;
	size_t pos;
	if (msg.find("PING") == std::string::npos && msg.find("PONG") == std::string::npos)
		std::cout << "Received(" << _fd << ") : " << msg << std::endl;
	if ((pos = msg.find_first_of(' ')) == std::string::npos) {
		ft_send(this->getFd(), ERR_NEEDMOREPARAMS(msg));
		// _server->broadcast(this, msg); // for testing purposes
		// send an error ERR_NEEDMOREPARAMS (461) ?
// I think this is the good error cause the irssi like other clients send always precision with msg send before the text
// but be careful about PING, and maybe some others cmd
// I think we should take the entiere string on getCommand if there is'nt any spaces
	// and formulate the error on the keywords' functions for this case
		return ;
	}
	funPtr f = _server->getCommand(msg.substr(0, pos));
	if (f != NULL)
		f(this, msg.substr(pos + 1, msg.size()));
	// else
	// 	std::cout << msg << std::endl;
		// _server->broadcast(this, msg); // for testing purposes
// BROADCAST 
// actually the iterator start with the current client ?
// the broadcast should be use since _server->client ?
// but exclude the current fd-client like now
}

bool Client::getStatus(void) const { return _passwd && !_nick.empty() && !_real.empty() && !_user.empty(); }

std::string Client::getHost() const { return _host; }

std::string Client::getPrefix() const { 
	return (_nick.empty() ? "*" \
		: (_nick + (_user.empty() ? "" : "!" + _user) +
		(_host.empty() ? "" : "@" + _host)));
}

std::string Client::getReal() const { return _real; }

bool Client::getResponse() const { return _response; }

bool Client::getPasswd() const { return _passwd; }

void Client::setStatus(void) { _clientReady = !_clientReady; }

void Client::setResponse(void) {
	_response = !_response;
/*	if (_response)// choose ur message
	{
// 		std::string end = "001 " + _user + " " + _nick + " :Welcome to the Internet Relay Network " + _nick + "\r\n";
// std::cerr << "debug CAP END : " + end;
// 		if (send(_fd, end.c_str(), end.size(), 0) != (ssize_t) end.size())
// 			std::cerr << "Error sending msg" << std::endl;
		std::string wel = "001 " + _nick + " :Welcome to the <networkname> Network, " + this->getPrefix() + "\r\n";
		std::cerr << "debug CAP END : " + wel;
		if (send(_fd, wel.c_str(), wel.size(), 0) != (ssize_t) wel.size())
			std::cerr << "Error sending msg" << std::endl;

//   "002 <client> :Your host is <servername>, running version <version>"
		wel = "003 " + _nick + " :This server was created " + _server->getCreatedTime() + "\r\n";
		if (send(_fd, wel.c_str(), wel.size(), 0) != (ssize_t) wel.size())
			std::cerr << "Error sending msg" << std::endl;
	}*/
}

void Client::setHost(std::string h) { _host = h; }
void Client::setReal(std::string r) { _real = r; }

