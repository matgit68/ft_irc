#include "Channel.hpp"

Channel::Channel(std::string n): _name(n), _topic("TopicTest") {}

Channel::Channel(std::string n, std::string w): _name(n), _passwd(w) {}

Channel::~Channel() {}

std::string Channel::getName() const { return _name; }

std::string Channel::getTopic() const { return _topic; }

std::string Channel::getMode() const { return _mode; }

std::string Channel::getPasswd(void) const { return _passwd; }

std::set<int> Channel::getClientList(void) const { return _clients; }


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
		return ; // client already on chan -> no ERR to send
	if (_mode.find('i') != std::string::npos && _invite.find(client->getFd()) == _invite.end()) // mode invite_only is set
		return ft_send(client->getFd(), ERR_INVITEONLYCHAN(_name));
	if ((_passwd.empty() || key == _passwd)) {
		_clients.insert(client->getFd());
		ft_send(client->getFd(), RPL_TOPIC);
		ft_send(client->getFd(), RPL_NAMREPLY);
		sendChan(client, RPL_JOIN_NOTIF(client->getNick(), _name));
	}
	else
		return ft_send(client->getFd(), ERR_BADCHANNELKEY);
}

void Channel::addClientInvite(Client *client) {
	if (!this->isInvite(client->getFd()))
		return ;
	if (_clients.find(client->getFd()) != _clients.end())
		return ;
	_clients.insert(client->getFd());
	ft_send(client->getFd(), RPL_TOPIC);
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

void Channel::sendChan(Client *client, std::string msg) const {
	// std::cout << "Sent(";
	// for (std::set<int>::iterator it = _clients.begin(); it != _clients.end(); )
	// {
	// 	std::cout << *it;
	// 	it++;
	// 	if (it != _clients.end())
	// 		std::cout << ", ";
	// }
	// if (msg[0] != ':')
	// 	msg = ":" + msg;	// easy syntax from NC but not totaly correct, to discuss
	// msg =  ":" + client->getNick() + " PRIVMSG " + _name + " " + msg + "\r\n";
	// std::cout << ") : " << msg << std::endl;
	for (std::set<int>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*it != client->getFd())
			ft_send(*it, msg);
			// send(*it, msg.c_str(), msg.size(), 0);
	}
}