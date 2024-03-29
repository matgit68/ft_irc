#pragma once
#include "hpp.hpp"

class Server;
class Client;

class Channel{
private:
/* 
	Channel names are strings (beginning with specified prefix characters). 
	Apart from the requirement of the first character being a valid channel
	type prefix character; the only restriction on a channel name is that it 
	may not contain any spaces (' ', 0x20), a control G / BELL ('^G', 0x07), 
	or a comma (',', 0x2C) (which is used as a list item separator by the protocol).

	regular channel - which is known to all servers that are connected to the network- prefix character for this type of channel is ('#', 0x23)
	local channels - where the clients connected can only see and talk to other clients on the same server - prefix character for this type of channel is ('&', 0x26)
*/
	std::string _name, _topic, _mode, _passwd; // mode could be a string containing "itkl"
	int _limit;
	std::set <int> _clients; // list of clients using the channel
	std::set <int> _ops; // list of clients ops on the channel
	std::set <int> _invite; // list of clients invited
	std::set<int>::iterator _it;
	Channel(Channel const &ref);
	Channel &operator=(Channel const &ref);
	Server *_server;

public:
	Channel(Server *, std::string); // topic unique name
	Channel(Server *, std::string, std::string); // name + pass
	~Channel();
	std::string getName() const;
	std::string getTopic() const;
	std::string getMode() const;
	std::string getPasswd() const;
	Server *getServer();
	std::set<int> getClientList(void) const;
	void setTopic(std::string);

	void addMode(Client *, char, std::string &);
	void unMode(Client *, char, std::string &);
	bool isOp(int);
	void giveOp(int); // give op privilege to a client identified by his name
	void removeOp(int); // remove op privilege to a client identified by his name

	void addClient(Client *, std::string);
	void delClient(Client *);
	bool isClient(Client *) const;

	void sendChan(Client *, std::string) const;
	void sendClients(std::string) const;
	void sendOps(std::string) const;
	void sendWhenArriving(Client *) const;

	void addClientInvite(Client *);

	void addInvite(int);
	void delInvite(int);
	bool isInvite(int) const;
};