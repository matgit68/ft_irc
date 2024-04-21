#pragma once
#include "hpp.hpp"

class Bot: public Client {
private:
	struct sockaddr_in _serverAddr;
	std::set<std::string> _persistence;
	std::map<std::string, std::string> _owner;
	int _sockFd;

public:
	Bot(Server *);
	~Bot();

	void setFd(int);
	void connectToServ();
	void sendToServ(std::string);
	void join(std::string, std::string);
	void react(std::string);
	void parse(std::string);
	void keepMode(Client *, std::string, bool);
	void keep(std::string);
	void op(Client *, std::string);
	std::string helpMsg();
};