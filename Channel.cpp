#include "Channel.hpp"

Channel::Channel(Server *s, std::string n): _name(n), _topic(""), _server(s) {}

Channel::Channel(Server *s, std::string n, std::string w): _name(n), _passwd(w), _server(s) {}

Channel::~Channel() {}

std::string Channel::getName() const { return _name; }

std::string Channel::getTopic() const { return _topic; }

std::string Channel::getMode() const { return _mode; }

std::string Channel::getRPLMode(int client) const {
	std::string pass;
	std::string RPLmode;

	if (_clients.find(client) == _clients.end()) // client is not member of the channel
		pass = "[key]";
	else
		pass = _passwd;
	RPLmode = _mode;
	if (RPLmode.find('k') != NPOS)
		RPLmode += ' ' + pass;
	if (RPLmode.find('l') != NPOS) {
		std::stringstream ss;
		ss << _limit;
		RPLmode += ' ' + ss.str();
	}
	return RPLmode;
}

std::string Channel::getPasswd(void) const { return _passwd; }

Server *Channel::getServer() { return _server; }

std::set<int> Channel::getClientList(void) const { return _clients; }

std::string Channel::getClientListbyName() const {
	std::set<int> list = getClientList();
	std::set<int>::iterator it;
	std::string result;

	for (it = list.begin(); it != list.end(); it++) {
		if (isOp(*it))
			result = '@' + _server->getClient(*it)->getNick() + ' ' + result;
		else
			result += ' ' + _server->getClient(*it)->getNick();
	}
	return result;
}

void Channel::setTopic(std::string t) { _topic = t; }

void Channel::delClient(Client *client) {
	if (_clients.find(client->getFd()) != _clients.end())
		_clients.erase(client->getFd());
}

void Channel::addInvite(int fd) { _invite.insert(fd); }

void Channel::delInvite(int fd) {
	if (_invite.find(fd) != _invite.end())
		_invite.erase(fd);
}

bool Channel::isClient(Client *search) const { return (_clients.find(search->getFd()) != _clients.end()); }

bool Channel::isInvited(int search) const {	return (_invite.find(search) != _invite.end()); }

bool Channel::empty() {	return (_clients.empty()); }

void Channel::addMode(Client *client, char mode, std::string &arg) {
	std::string authMode = "+-itklo";
	if (authMode.find(mode) == NPOS)
		return _server->ft_send(client->getFd(), ERR_UMODEUNKNOWNFLAG());
	if (mode == 'o') { // give op privileges
		std::string target = takeNextArg(arg);
		if (target.empty())
			return _server->ft_send(client->getFd(), ERR_NEEDMOREPARAMS("MODE"));
		Client *c = _server->getClient(target);
		if (c == NULL)
			return _server->ft_send(client->getFd(), ERR_NOSUCHNICK(client, target));
		if (_clients.find(c->getFd()) == _clients.end())
			return _server->ft_send(client->getFd(), ERR_USERNOTINCHANNEL(client, target, _name));
		giveOp(c->getFd());
		return sendChan(NULL, RPL_UMODEINCHANIS(client, this, "+o", c));
	}
	if (mode == 'i' && _mode.find('i') == NPOS) { // invite mode
		_mode.append("i");
		return sendChan(NULL, RPL_MODE(client, this, "+i"));
	}
	if (mode == 't' && _mode.find('t') == NPOS) {// protected topic mode
		_mode.append("t");
		return sendChan(NULL, RPL_MODE(client, this, "+t"));
	}
	if (mode == 'k') { // key protected mode (password)
		if (arg.empty())
			return;
		if (_mode.find('k') == NPOS)
			_mode.append("k");
		_passwd = takeNextArg(arg);
		return sendChan(NULL, RPL_MODEPWD(client, this, mode, arg));
	}
	if (mode == 'l') { // user limit
		std::string tmp = takeNextArg(arg);
		if (tmp.find_first_not_of("01234546789") != NPOS || tmp.size() > 3)
			return;
		_limit = std::atoi(tmp.c_str());
		if (_limit > 50)
			_limit = 50;
		if (_mode.find('l') == NPOS)
			_mode.append("l");
		return sendChan(NULL, RPL_MODELIM(client, this, mode, tmp));
	}
}

void Channel::unMode(Client *client, char mode, std::string &arg) {
	size_t pos;
	if (mode == 'o') {
		Client *c = _server->getClient(takeNextArg(arg));
		if (c != NULL) {
			removeOp(c->getFd());
			sendChan(NULL, RPL_UMODEINCHANIS(client, this, "-o", c));
		}
		else
			_server->ft_send(client->getFd(), ERR_NOSUCHNICK(client, arg));
	}
	else {
		if ((pos = _mode.find(mode)) != NPOS) {
			_mode.erase(pos, 1);
			return sendChan(NULL, RPL_MODE(client, this, "-" + mode));
		}
	}
}

void Channel::sortMode() {	std::sort(_mode.begin(), _mode.end()); }

void Channel::sendModeInfo(Client *client) const {
	_server->ft_send(client->getFd(), RPL_CHANNELMODEIS(client, this, getRPLMode(client->getFd())));
}

void Channel::sendModeInfo() const {
	std::set<int>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
		sendModeInfo(_server->getClient(*it));
}

bool Channel::isOp(int id) const {
	if (_ops.find(id) != _ops.end())
		return true;
	return false;
}

void Channel::giveOp(int id) {
	if (_ops.find(id) == _ops.end())
		_ops.insert(id);
}

void Channel::removeOp(int id) {
	if (_ops.find(id) != _ops.end())
		_ops.erase(id);
}

void Channel::addClient(Client *client) {
	if (_clients.find(client->getFd()) != _clients.end()) // client already on chan -> nothing to do
		return ;
	if (_mode.find('l') != NPOS && _clients.size() >= _limit)
		return _server->ft_send(client->getFd(), ERR_CHANNELISFULL(this));
	_clients.insert(client->getFd()); // no mode is set, just add the client to the channel clients list
	sendWhenJoining(client);
}

void Channel::addClientInvite(Client *client) { // no chan limit nor password required here 
	if (_clients.find(client->getFd()) != _clients.end()) // client is already on the channel
		return ;
	_clients.insert(client->getFd());
	sendWhenJoining(client);
	this->delInvite(client->getFd());
}

void Channel::addClientPass(Client *client, std::string key) {
	if (_clients.find(client->getFd()) != _clients.end()) // client already on chan -> nothing to do
		return ; 
	if (_mode.find('l') != NPOS && _clients.size() >= _limit )
		return _server->ft_send(client->getFd(), ERR_CHANNELISFULL(this));
	if (key != _passwd)
		return _server->ft_send(client->getFd(), ERR_BADCHANNELKEY(client, this)); // wrong password
	_clients.insert(client->getFd()); // no mode is set, just add the client to the channel clients list
	sendWhenJoining(client);
}

void Channel::sendWhenJoining(Client *client) const {
	sendChan(NULL, RPL_JOIN_NOTIF(client->getNick(), _name)); //send all users
	if (!_topic.empty())
		_server->ft_send(client->getFd(), RPL_TOPIC(client, this));
	_server->ft_send(client->getFd(), RPL_NAMREPLY(client, this, getClientListbyName()));
	_server->ft_send(client->getFd(), RPL_ENDOFNAMES(client, this));
}

// If client is set, send msg to all clients connected to the chan EXCEPT the sender
// If client is NULL, send to ALL clients, even the sender
void Channel::sendChan(Client *client, std::string msg) const {
	int fd = 0;
	if (client != NULL)
		fd = client->getFd();
	for (std::set<int>::iterator it = _clients.begin(); it != _clients.end(); it++)	{
		if (*it != fd)
			_server->ft_send(*it, msg);
	}
}

void Channel::removeUser( Client *client ){
	delInvite(client->getFd());
	removeOp(client->getFd());
	delClient(client);
	if (!_server->getBotname().empty() && _clients.size() == 1)
		_server->ft_send(_server->getClient(_server->getBotname())->getFd(), "LAST:" + _name + "\r\n");
}