#include "Channel.hpp"

Channel::Channel(std::string n): _name(n) {}

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

void Channel::addClient(Client *client) {
	if (_clients.find(client->getFd()) == _clients.end())
		_clients.insert(client->getFd());
}

void Channel::delClient(Client *client) {
if (_clients.find(client->getFd()) != _clients.end())
	_clients.erase(client->getFd());
}
