#pragma once
#include "hpp.hpp"

typedef void (*funPtr)(Client *, std::string);

class Bot;

class Server {
private:
	int _fd, _epollfd, _port, _keeperFd;
	std::string _passwd, _hostname, _createdTime, _botname;
	struct sockaddr_in _address;
	struct epoll_event _ev, _events[MAX_EVENTS];
	std::map <int, Client*> _clients;
	std::map <std::string, Channel*> _channels;
	std::map <std::string, funPtr> _commands;
	std::map <std::string, funPtr> _botcmd;
	Bot *_keeper;
	Server();

public:
	Server(int, std::string, std::string);
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
	std::string getBotname() const;
	int getBotFd() const;
	void setHostname(std::string);
	std::map<std::string, Channel*> getChannelMap(void) const;
	funPtr getCommand(std::string);
	
	std::string getCreatedTime(void) const;

	void ft_send(int, std::string);
	void delClient(int);
	void disconnectClient(int);
	void createChannel(std::string, Client *);
	Channel * addChannel(std::string);
	Channel * addChannel(std::string, std::string);
	void delChannel(std::string);
	void dispChannels(Client *);
	void checkEmptyChannels();
	void sendToClientsInTouch(Client *, std::string, bool);
	void removeFromAllChannels(Client *);

	void initFunPtr();
	void init();
	void run();
	void sendRegistration(Client *);
	bool isNickAvailable(std::string& newNick);

	bool userOnChannel(int, std::string);
	bool findChannel(std::string channel);
	
};