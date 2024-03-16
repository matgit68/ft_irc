#include "Channel.hpp"

Channel::Channel(std::string n): _name(n) {}

Channel::Channel(std::string n, std::string w): _name(n), _passwd(w) {}

Channel::~Channel() {}

std::string Channel::getName() const { return _name; }

std::string Channel::getTopic() const { return _topic; }

void Channel::setTopic(std::string t) { _topic = t; }

bool Channel::isOp(int id) {
	if (_op.find(id) != _op.end())
		return true;
	return false;
}

void Channel::giveOp(int id) {
	if (_op.find(id) == _op.end())
		_op.insert(id);
}

void Channel::removeOp(int id) {
	if (_op.find(id) != _op.end())
		_op.erase(id);
}

void Channel::addClient(Client *client, std::string key) {
	if (_clients.find(client->getFd()) != _clients.end())
		return ; // ERR client already on chan
	else if (((_passwd.empty() && key.empty()) || key == _passwd)
			&& _clients.find(client->getFd()) == _clients.end())
		_clients.insert(client->getFd());
	else if (!key.empty())
		return ; // ERR bad password
}

void Channel::delClient(Client *client) {
if (_clients.find(client->getFd()) != _clients.end())
	_clients.erase(client->getFd());
}

std::string Channel::getPasswd(void) const { return _passwd; }