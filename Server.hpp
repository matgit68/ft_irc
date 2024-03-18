#pragma once

#include "hpp.hpp"

class Client;
class Channel;

typedef void (*funPtr)(Client *, std::string);

class Server {
private:
	int _fd, _port;
	std::string _passwd, _hostname, _createdTime;
	struct sockaddr_in _address;
	struct epoll_event _ev, _events[MAX_EVENTS];
	std::map <int, Client*> _clients;
	std::map <std::string, Channel*> _channels;
	std::map <std::string, funPtr> _commands;
	Server();

public:
	Server(int, std::string);
	~Server();
	Server(Server const &ref);
	Server &operator=(Server const &ref);

	int getFd() const;
	int getPort() const;
	std::string getPasswd() const;
	Client *getClient(int);
	Channel *getChannel(std::string);
	funPtr getCommand(std::string);
	
	std::string getCreatedTime(void) const;

	void initFunPtr();
	void init();
	void run();
	void broadcast(Client*, std::string);
	void sendRegistration(Client *);
	bool isNickAvailable(std::string& newNick);
};