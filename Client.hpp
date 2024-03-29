#pragma once

#include <string>
#include <vector>
#include "Server.hpp"

class Server;

class Client{
private:
	std::string _user, _nick, _real, _buffer, _host, _srvaddr, _oldnick;
	Server *_server;
	int _fd;
	Client(Client const &ref);
	Client &operator=(Client const &ref);
	bool _clientReady, _response, _passwd; // i had a pb with only clientReady, so i added _response, i havn't test again now, so i don't no if ze need to add this again.

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
	std::string getOldNick() const;
	void setUser(std::string);
	void setNick(std::string);
	void setOldNick(std::string);
	void setReal(std::string);
	void setHost(std::string);
	void setPasswd(bool);
	void setStatus(void);
	void setResponse(void);
	void receive(char*);
	void parse(std::string);
};