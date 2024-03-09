#pragma once

#include <string>
#include "Server.hpp"

class Server;

class Client{
private:
	std::string user, nick, buffer;
	Server *server;
	int fd;
	Client(Client const &ref);
	Client &operator=(Client const &ref);
	bool clientReady;

public:
	Client(int);
	~Client();
	int getFd() const;
	std::string getUser() const;
	std::string getNick() const;
	void setUser(std::string);
	void setNick(std::string);
	void receive(Server*, char*);
	void parse(Server*);
};