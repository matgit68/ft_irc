#pragma once

#include <string>
#include <set>
#include "Client.hpp"

class Client;

class Channel{
private:
	std::string name, topic, mode, passwd; // mode could be a string containing "itkol"
	std::set <std::string> clients; // list of clients using the channel
	std::set <std::string> op; // list of clients ops on the channel
	Channel(Channel const &ref);
	Channel &operator=(Channel const &ref);

public:
	Channel(std::string); // topic unique name
	~Channel();
	std::string getName() const;
	std::string getTopic() const;
	void setTopic(std::string);

	bool isOp(std::string);
	void giveOp(std::string); // give op privilege to a client identified by his name
	void removeOp(std::string); // remove op privilege to a client identified by his name
};