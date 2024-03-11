#pragma once

#include <string>
#include "Server.hpp"

class Server;

class Client{
private:
	std::string _user, _nick, _real, _buffer, _host;
	Server *_server;
	int _fd;
	Client(Client const &ref);
	Client &operator=(Client const &ref);
	bool _clientReady, _response;

public:
	Client(int, Server*);
	~Client();
	bool getStatus() const;
	bool getResponse() const;
	int getFd() const;
	std::string getUser() const;
	std::string getNick() const;
	std::string getReal() const;
	std::string getHost() const;
	std::string getPrefix() const;
	void setUser(std::string);
	void setNick(std::string);
	void setReal(std::string);
	void setHost(std::string);
	void setStatus(void);
	void setResponse(void);
	void receive(char*);
	// void receiveV2(char* str);
	void parse(std::string);
};