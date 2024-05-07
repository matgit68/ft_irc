#pragma once
#include "hpp.hpp"

class Channel{
private:
	std::string _name, _topic, _mode, _passwd; // mode could be a string containing "itkl"
	size_t _limit;
	std::set <int> _clients; // list of clients using the channel
	std::set <int> _ops; // list of clients ops on the channel
	std::set <int> _invite; // list of clients invited
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
	std::string getRPLMode(int) const;
	std::string getPasswd() const;
	std::string getClientListbyName() const;
	Server *getServer();
	std::set<int> getClientList(void) const;
	void setTopic(std::string);
	bool empty();

	void addMode(Client *, char, std::string &);
	void unMode(Client *, char, std::string &);
	void sortMode();
	bool isOp(int) const;
	void giveOp(int); // give op privilege to a client identified by his name
	void removeOp(int); // remove op privilege to a client identified by his name

	void addClientPass(Client *, std::string);
	void addClient(Client *);
	void delClient(Client *);
	bool isClient(Client *) const;

	void sendChan(Client *, std::string) const;
	void sendWhenJoining(Client *) const;
	void sendModeInfo() const;
	void sendModeInfo(Client *) const;

	void addClientInvite(Client *);

	void addInvite(int);
	void delInvite(int);
	bool isInvited(int) const;

	void removeUser(Client *);
};