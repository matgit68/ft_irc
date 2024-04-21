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

void Bot::join(std::string chanName, std::string owner) {
	sendToServ("JOIN " + chanName);
	_owner[chanName] = owner;
	_persistence.insert(chanName);
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
	std::string user;
	Client *target;

	if (msg.find("PRIVMSG " BOTNAME) != NPOS) { // PRIVMSG == commands
		user = takeNextArg(msg);
		user.erase(0, 1);
		msg.erase(0,1);
		target = _server->getClient(user);
		msg.erase(0, msg.find(':') + 1);

		std::string cmd = takeNextArg(msg);
		if (cmd == "!keep")
			keepMode(target, msg, true);
		else if (cmd == "!nokeep")
			keepMode(target, msg, false);
		else if (cmd == "!op")
			op(target, msg);
		else
			_server->ft_send(target->getFd(), RPL_PRIVUSERMSG(this, target, helpMsg()));
	}
	else if (msg.compare(0, 5, "LAST:") == 0) // react to chan events
		keep(msg.erase(0,5));
}

std::string Bot::helpMsg() {
	return "Commands are : !keep <channel>, !nokeep <channel>, !op <channel>, !owner <channel>";
}

void Bot::keepMode(Client *target, std::string chanName, bool keeping) {
	Channel *channel = _server->getChannel(chanName);
	if (channel == NULL)
		_server->ft_send(target->getFd(), RPL_PRIVUSERMSG(this, target, "Channel " + chanName + "not found"));
	else if (keeping) {
		_persistence.insert(chanName);
		_server->ft_send(target->getFd(), RPL_PRIVUSERMSG(this, target, "I will keep " + chanName + " from disappearance"));
	}
	else if (!keeping && _persistence.find(chanName) != _persistence.end()) {
		_persistence.erase(chanName);
		_server->ft_send(target->getFd(), RPL_PRIVUSERMSG(this, target, chanName + " is no longer protected"));
	}
}

void Bot::op(Client *target, std::string chanName) {
	Channel *channel = _server->getChannel(chanName);
	if (_owner[chanName] == target->getNick()) {
		channel->giveOp(target->getFd());
		channel->sendChan(this, RPL_UMODEINCHANIS(this, channel, "+o", target));
	}
	else
		_server->ft_send(target->getFd(), RPL_PRIVUSERMSG(this, target, "You don't own this channel"));
}

void Bot::keep(std::string chanName) {
	if (_persistence.find(chanName) == _persistence.end()) {
		_owner.erase(chanName);
		sendToServ("PART " + chanName);
	}
}