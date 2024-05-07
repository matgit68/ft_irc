#include "Client.hpp"

Client::Client(int f, Server *s): _host("default"), _server(s), _fd(f), _response(false), _passwd(false) {}

Client::~Client() {}

int Client::getFd() const { return _fd; }

std::string Client::getUser() const { return _user; }

std::string Client::getNick() const { return _nick; }

std::string Client::getOldNick() const { return _oldnick; }

Server *Client::getServer() const { return _server; }

std::string Client::getReal() const { return _real; }

bool Client::getResponse() const { return _response; }

bool Client::getPasswd() const { return _passwd; }

bool Client::getStatus(void) const { return _passwd && !_nick.empty() && !_real.empty() && !_user.empty(); }

std::string Client::getHost() const { return _host; }

std::string Client::getPrefix() const { 
	return (_nick.empty() ? "*" \
		: (_nick + (_user.empty() ? "" : "!" + _user) +
		(_host.empty() ? "" : "@" + _host)));
}

void Client::setUser(std::string u) { _user = u; }

void Client::setNick(std::string n) { _nick = n; }

void Client::setPasswd(bool b) { _passwd = b; }

void Client::setResponse(bool b) { _response = b; }

void Client::setOldNick(std::string nickname) { _oldnick = nickname; }

void Client::setHost(std::string h) { _host = h; }

void Client::setReal(std::string r) { _real = r; }

void Client::receive(char* str) { 
	_buffer.append(str);
	if (_buffer.size() > 512 ) { // Servers SHOULD gracefully handle messages over the 512-bytes limit. They may send an error numeric back, preferably ERR_INPUTTOOLONG (417) 
		_server->ft_send(_fd, ERR_INPUTTOOLONG(this));
		_buffer.clear();
	}
	size_t pos;
	while ((pos = _buffer.find("\r\n")) != NPOS) {
		if (parse(_buffer.substr(0, pos)))
			_buffer.erase(0, pos + 2);
		else
			break;
	}
}

int Client::parse(std::string msg) {
	bool authCmd = false;
	trim(msg);
	if (msg.find("PING") == NPOS && msg.find("PONG") == NPOS) {
		if (!_server->getBotname().empty() && _fd == _server->getBotFd())
			std::cout << GREEN "<<(" << _fd << ") : " BOT << msg << RESET << std::endl;
		else
			std::cout << GREEN "<<(" << _fd << ") : " RESET << msg << std::endl;
	}
	std::string cmd = takeNextArg(msg);
	if (!cmd.compare("PASS") || !cmd.compare("NICK") || !cmd.compare("USER") || !cmd.compare("QUIT"))
		authCmd = true;

	funPtr f = _server->getCommand(cmd);
	if (getStatus() || authCmd) {
		if (f != NULL)
			f(this, msg);
		else
			_server->ft_send(_fd, ERR_UNKNOWNCOMMAND(this, cmd));
	}
	return (cmd.compare("QUIT"));
}