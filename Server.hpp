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
	Client *getClient(std::string) const;
	Channel *getChannel(std::string);
	std::string getHostname() const;
	void setHostname(std::string);
	std::map<std::string, Channel*> getChannelMap(void) const;
	funPtr getCommand(std::string);
	
	std::string getCreatedTime(void) const;

	void createChannel(std::string, Client *);
	Channel * addChannel(std::string);
	Channel * addChannel(std::string, std::string);
	void delChannel(std::string);
	void dispChannels(Client *);
	void makeQuit(int);

	void initFunPtr();
	void init();
	void run();
	void broadcast(Client*, std::string);
	void sendRegistration(Client *);
	bool isNickAvailable(std::string& newNick);

	bool userOnChannel(int, std::string);
	bool findChannel(std::string channel);
	std::map<std::string, Channel*>	getChannels();
};