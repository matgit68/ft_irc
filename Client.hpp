#pragma once

#include <string>
#include <vector>
#include "Server.hpp"

class Server;

class Client{
private:
	std::string _user, _nick, _real, _buffer, _host, _param, _command;
	Server *_server;
	int _fd;
	Client(Client const &ref);
	Client &operator=(Client const &ref);
	bool _clientReady, _response, _passwd;

public:
	Client(int, Server*);
	~Client();
	bool getPasswd() const;
	bool getStatus() const;
	bool getResponse() const;
	int getFd() const;
	std::string getUser() const;
	std::string getNick() const;
	std::string getReal() const;
	std::string getHost() const;
	std::string getPrefix() const;
	Server *getServer() const;
	void setUser(std::string);
	void setNick(std::string);
	void setReal(std::string);
	void setHost(std::string);
	void setPasswd(bool);
	void setStatus(void);
	void setResponse(void);
	void receive(char*);
	void parse(std::string);
	std::string getCommand( void );
};