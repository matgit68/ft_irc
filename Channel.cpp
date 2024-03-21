#include "Channel.hpp"

Channel::Channel(Server *s, std::string n): _name(n), _topic("TopicTest"), _server(s) {}

Channel::Channel(Server *s, std::string n, std::string w): _name(n), _passwd(w), _server(s) {}

Channel::~Channel() {}

std::string Channel::getName() const { return _name; }

std::string Channel::getTopic() const { return _topic; }

std::string Channel::getMode() const { return _mode; }

std::string Channel::getPasswd(void) const { return _passwd; }

Server *Channel::getServer() { return _server; }

std::set<int> Channel::getClientList(void) const { return _clients; }

void Channel::setTopic(std::string t) { _topic = t; }

void Channel::addMode(Client *client, char mode, std::string &arg) {
	std::string authMode = "+-itklo";
	if (authMode.find(mode) == NPOS)
		return ft_send(client->getFd(), ERR_UMODEUNKNOWNFLAG());
	if (mode == 'i' && _mode.find('i') == NPOS) // invite mode
		_mode.append("i");
	if (mode == 't' && _mode.find('t') == NPOS) // protected topic mode
		_mode.append("t");
	if (mode == 'k') { // key protected mode (password)
		if (_mode.find('k') == NPOS)
			_mode.append("k");
		_passwd = takeNextArg(arg);
	}
	if (mode == 'l') { // user limit
		if (_mode.find('l') == NPOS)
			_mode.append("l");
		_limit = atoi(takeNextArg(arg).c_str());
	}
	if (mode == 'o') { // give op privileges
		std::string target = takeNextArg(arg);
		if (target.empty())
			return ft_send(client->getFd(), ERR_NEEDMOREPARAMS("MODE"));
		Client *c = _server->getClient(target);
		if (c == NULL)
			return ft_send(client->getFd(), ERR_NOSUCHNICK(client->getNick(), target));
		if (_clients.find(c->getFd()) == _clients.end())
			return ft_send(client->getFd(), ERR_USERNOTINCHANNEL(client, target, _name));
		giveOp(c->getFd());
	}
}

void Channel::unMode(Client *client, char mode, std::string &arg) {
	size_t pos;
	if (mode == 'o') {
		Client *c = _server->getClient(takeNextArg(arg));
		if (c != NULL)
			removeOp(c->getFd());
		else
			ft_send(client->getFd(), ERR_NOSUCHNICK(client->getNick(), arg));
	}
	else
		if ((pos = _mode.find(mode)) != NPOS)
			_mode.erase(pos, 1);
}

bool Channel::isOp(int id) {
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

void Channel::addClient(Client *client, std::string key) {
	if (_clients.find(client->getFd()) != _clients.end())
		return ; // client already on chan -> nothing to do
	if (_mode.find('i') != NPOS && _invite.find(client->getFd()) == _invite.end()) // mode invite_only is set and client is not invited
		return ft_send(client->getFd(), ERR_INVITEONLYCHAN(_name));
	if (_mode.find('i') != NPOS && _invite.find(client->getFd()) != _invite.end()) { // mode invite_only is set and client is invited (overriding passwd verif)
		_clients.insert(client->getFd());
		return sendWhenArriving(client);
		//ft_send(client, RPL_TOPIC);
		sendChan(client, RPL_NAMREPLY);		
	}
	if (_mode.find('k') != NPOS) { // mode key protected channel is set
		if (_passwd.empty() || key == _passwd) {
			_clients.insert(client->getFd());
			return sendWhenArriving(client);
		}
		else
			return ft_send(client->getFd(), ERR_BADCHANNELKEY); // wrong password
	}
	_clients.insert(client->getFd()); // no mode is set, just add the client to the channel clients list
	return sendWhenArriving(client);
}

void Channel::sendWhenArriving(Client *client) const {
	//ft_send(client->getFd(), RPL_TOPIC);
	ft_send(client->getFd(), RPL_NAMREPLY);
	sendChan(client, RPL_JOIN_NOTIF(client->getNick(), _name));
}

void Channel::addClientInvite(Client *client) {
	if (!this->isInvite(client->getFd()))
		return ;
	if (_clients.find(client->getFd()) != _clients.end())
		return ;
	_clients.insert(client->getFd());
	//ft_send(client->getFd(), RPL_TOPIC);
	ft_send(client->getFd(), RPL_NAMREPLY);
	sendChan(client, RPL_JOIN_NOTIF(client->getNick(), _name));
	this->delInvite(client->getFd());
}

void Channel::delClient(Client *client) {
	if (_clients.find(client->getFd()) != _clients.end())
		_clients.erase(client->getFd());
}

void Channel::addInvite(int fd) {
	_invite.insert(fd);
}

void Channel::delInvite(int fd) {
	if (_invite.find(fd) != _invite.end())
		_invite.erase(fd);
}

bool Channel::isClient(Client *search) const {
	if (_clients.find(search->getFd()) != _clients.end())
		return (true);
	return (false);
}

bool Channel::isInvite(int search) const {
	if (_invite.find(search) != _invite.end())
		return (true);
	return (false);
}

// Send msg to all clients connected to the chan except the sender
// If client* is NULL, will send to ALL clients, even the sender
void Channel::sendChan(Client *client, std::string msg) const {
	int fd = 0;
	if (client != NULL)
		fd = client->getFd();
	for (std::set<int>::iterator it = _clients.begin(); it != _clients.end(); it++)	{
		if (*it != fd)
			ft_send(*it, msg);
	}
}

void Channel::sendClients(std::string msg) const { // send msg to all clients except ops
	for (std::set<int>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (_ops.find(*it) == _ops.end())
			ft_send(*it, msg);
	}
}

void Channel::sendOps(std::string msg) const { // send msg to all ops
	for (std::set<int>::iterator it = _ops.begin(); it != _ops.end(); it++)
		ft_send(*it, msg);
}