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

std::string Channel::getChangedNick(int id, std::string user)
{
	if(isOp(id))
		return ("@" + user);
	else
		return (user);
}

void Channel::changeNick(std::string oldNick, std::string newNick){
	(void)oldNick;
	(void)newNick;
	//to be continued
}