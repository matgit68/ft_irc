#include "Channel.hpp"

Channel::Channel(Server *s, std::string n): _name(n), _topic(""), _server(s) {}

Channel::Channel(Server *s, std::string n, std::string w): _name(n), _passwd(w), _server(s) {}

Channel::~Channel() {}

std::string Channel::getName() const { return _name; }

std::string Channel::getTopic() const { return _topic; }

std::string Channel::getMode() const { return _mode; }

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
	if (mode == 'i' && _mode.find('i') == NPOS) // invite mode
		_mode.append("i");
	if (mode == 't' && _mode.find('t') == NPOS) // protected topic mode
		_mode.append("t");
	if (mode == 'k') { // key protected mode (password)
		if (_mode.find('k') == NPOS)
			_mode.append("k");
		if (!arg.empty())
			_passwd = takeNextArg(arg);
	}
	if (mode == 'l') { // user limit
		if (_mode.find('l') == NPOS)
			_mode.append("l");
		_limit = std::atoi(takeNextArg(arg).c_str());
	}
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
		sendChan(NULL, RPL_UMODEINCHANIS(client, this, "+o", c));
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
	else
		if ((pos = _mode.find(mode)) != NPOS)
			_mode.erase(pos, 1);
}

void Channel::sendModeInfo(Client *client) const {
// send mode infos to specific client
	if (_mode.find('k') == NPOS)
		_server->ft_send(client->getFd(), RPL_CHANNELMODEIS(client, this, getMode()));
	else if (isClient(client))
		_server->ft_send(client->getFd(), RPL_CHANNELMODEISWITHKEY(client, this, getPasswd()));
	else
		_server->ft_send(client->getFd(), RPL_CHANNELMODEISWITHKEY(client, this, "[key]"));
}

void Channel::sendModeInfo() const {
	std::set<int>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++) {
		if (_mode.find('k') == NPOS)
			_server->ft_send(*it, RPL_CHANNELMODEIS(_server->getClient(*it), this, getMode()));
		else
			_server->ft_send(*it, RPL_CHANNELMODEISWITHKEY(_server->getClient(*it), this, getPasswd()));
	}
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

void Channel::addClientInvite(Client *client) { // PAS DE LIMITES DE CHAN !
	if (_clients.find(client->getFd()) != _clients.end()) // client is already on the channel
		return ;
	if (!this->isInvited(client->getFd())) // client is not on the list
		return _server->ft_send(client->getFd(), ERR_INVITEONLYCHAN(this));
	_clients.insert(client->getFd());
	sendWhenJoining(client);
	this->delInvite(client->getFd());
}

void Channel::addClientPass(Client *client, std::string key) {
	std::cout << "KEY JOIN" << std::endl;
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

void Channel::sendChan(Client *client, std::string msg) const {
// If client is set, send msg to all clients connected to the chan EXCEPT the sender
// If client is NULL, send to ALL clients, even the sender
	int fd = 0;
	if (client != NULL)
		fd = client->getFd();
	for (std::set<int>::iterator it = _clients.begin(); it != _clients.end(); it++)	{
		if (*it != fd)
			_server->ft_send(*it, msg);
	}
}

void Channel::sendClients(std::string msg) const { // send msg to all clients except ops
	for (std::set<int>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (_ops.find(*it) == _ops.end())
			_server->ft_send(*it, msg);
	}
}

void Channel::sendOps(std::string msg) const { // send msg to all ops
	for (std::set<int>::iterator it = _ops.begin(); it != _ops.end(); it++)
		_server->ft_send(*it, msg);
}

void Channel::removeUser( Client *client ){
	delInvite(client->getFd());
	removeOp(client->getFd());
	delClient(client);
}