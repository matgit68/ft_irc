#include "Bot.hpp"

Bot::Bot(Server *s): Client(0, s) {
	_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFd == -1) 
		std::cerr << "Could not create socket" << std::endl;

	setNick(BOTNAME);
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(s->getPort());
	_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

Bot::~Bot() {}

// int Bot::getFd() const { return _fd; }

void Bot::setFd(int f) { _fd = f; }

void Bot::sendToServ(std::string msg) {
	msg += "\r\n";
	send(_sockFd, msg.c_str(), msg.size(), 0);
}

void Bot::connectToServ() {
	if (connect(_sockFd, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0)
		std::cerr << "Could not connect to server" << std::endl;

	std::cout << YELLOW "Bot [keeper] connected" RESET << std::endl;
	sendToServ("PASS " + _server->getPasswd());
	sendToServ("NICK " BOTNAME);
	sendToServ("USER " BOTNAME " 0 * " BOTNAME);
}

void Bot::react(std::string str) {
	_buffer.append(str);
	if (_buffer.size() > 512 )
		_buffer.clear();
	size_t pos;
	while ((pos = _buffer.find("\r\n")) != NPOS) {
		parse(_buffer.substr(0, pos));
		_buffer.erase(0, pos + 2);
	}
}

void Bot::parse(std::string msg) {
	std::string poke = "PRIVMSG " BOTNAME;
	std::string user;
	Client *target;
	if (msg.find(poke) != NPOS) { // commands mode
		user = takeNextArg(msg);
		user.erase(0, 1);
		msg.erase(0,1);
		target = _server->getClient(user);
		msg.erase(0, msg.find(':') + 1);

		std::string cmd = takeNextArg(msg);
		if (cmd.compare("!keep") == 0)
			keep(user, msg, 1);
		else if (cmd.compare("!nokeep") == 0)
			keep(user, msg, 0);
		else if (cmd.compare("!op") == 0)
			op(user, msg);
		else
			_server->ft_send(target->getFd(), RPL_PRIVUSERMSG(this, target, helpMsg()));
	}
	else if (msg.find(" 353 " BOTNAME) != NPOS) { // User list, setting owner
		std::cout << "Looking for owner" << std::endl;
	}
}

std::string Bot::helpMsg() {
	return "Commands are : !help, !keep <channel>, !nokeep <channel>, !op <channel>";
}

void Bot::keep(std::string nick, std::string chanName, bool keeping) {
	Channel *channel = _server->getChannel(chanName);
	Client *client = _server->getClient(nick);
	if (channel == NULL)
		_server->ft_send(client->getFd(), RPL_PRIVUSERMSG(this, client, "Channel " + chanName + "not found"));
	else if (keeping) {
		_persistence.insert(chanName);
		_server->ft_send(client->getFd(), RPL_PRIVUSERMSG(this, client, "I will keep " + chanName + " from disappearance"));
	}
	else if (!keeping && _persistence.find(chanName) != _persistence.end()) {
		_persistence.erase(chanName);
		_server->ft_send(client->getFd(), RPL_PRIVUSERMSG(this, client, chanName + " is no longer protected"));
	}
}

void Bot::op(std::string nick, std::string chanName) {
	(void) nick;
	(void) chanName;
}