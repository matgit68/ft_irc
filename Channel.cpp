#include "Channel.hpp"

Channel::Channel(std::string n): name(n) {}

Channel::~Channel() {}

std::string Channel::getName() const { return name; }

std::string Channel::getTopic() const { return topic; }

void Channel::setTopic(std::string t) { topic = t; }

bool Channel::isOp(std::string name) {
	if (op.find(name) != op.end())
		return true;
	return false;
}

void Channel::giveOp(std::string name) {
	if (op.find(name) == op.end())
		op.insert(name);
}

void Channel::removeOp(std::string name) {
	if (op.find(name) != op.end())
		op.erase(name);
}
