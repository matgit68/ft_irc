#include "Channel.hpp"

Channel::Channel(std::string n): _name(n) {}

Channel::Channel(std::string n, std::string w): _name(n), _passwd(w) {}

Channel::~Channel() {}

std::string Channel::getName() const { return _name; }

std::string Channel::getTopic() const { return _topic; }

std::string Channel::getPasswd(void) const { return _passwd; }

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

bool Channel::isClient(Client *search) const {
	if (_clients.find(search->getFd()) != _clients.end())
		return (true);
	return (false);
}

void Channel::sendChan(Client *client, std::string msg) const {
	std::cout << "Send(";
	for (std::set<int>::iterator it = _clients.begin(); it != _clients.end(); )
	{
		std::cout << *it;
		it++;
		if (it != _clients.end())
			std::cout << ", ";
	}
	if (msg[0] != ':')
		msg = ":" + msg;	// easy syntax from NC but not totaly correct, to discuss
	msg =  ":" + client->getNick() + " PRIVMSG " + _name + " " + msg + "\r\n";
	std::cout << ") : " << msg << std::endl;
	for (std::set<int>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*it != client->getFd())
			send(*it, msg.c_str(), msg.size(), 0);
	}
}