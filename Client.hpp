#pragma once
#include "hpp.hpp"

class Client{
protected:
	std::string _user, _nick, _real, _buffer, _host, _oldnick;
	Server *_server;
	int _fd;
	Client(Client const &ref);
	Client &operator=(Client const &ref);
	std::set<std::string> _channels;
	bool _response, _passwd;

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
	void setResponse(bool);
	void receive(char*);
	int parse(std::string);
};