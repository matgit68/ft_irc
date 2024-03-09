#pragma once

#include <string>
#include <set>
#include "Client.hpp"

class Client;

class Channel{
private:
	std::string _name, _topic, _mode, _passwd; // mode could be a string containing "itkol"
	std::set <int> _clients; // list of clients using the channel
	std::set <int> _op; // list of clients ops on the channel
	Channel(Channel const &ref);
	Channel &operator=(Channel const &ref);

public:
	Channel(std::string); // topic unique name
	~Channel();
	std::string getName() const;
	std::string getTopic() const;
	void setTopic(std::string);

	bool isOp(int);
	void giveOp(int); // give op privilege to a client identified by his name
	void removeOp(int); // remove op privilege to a client identified by his name
};