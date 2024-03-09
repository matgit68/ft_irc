#pragma once

#include <string>
#include "Server.hpp"

class Server;

class Client{
private:
	std::string _user, _nick, _buffer;
	Server *_server;
	int _fd;
	Client(Client const &ref);
	Client &operator=(Client const &ref);
	bool _clientReady;

public:
	Client(int, Server*);
	~Client();
	int getFd() const;
	std::string getUser() const;
	std::string getNick() const;
	void setUser(std::string);
	void setNick(std::string);
	void receive(char*);
	void parse(std::string);
};