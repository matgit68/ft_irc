#pragma once

#include <stdio.h>
#include <string.h>//strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>//close
#include <arpa/inet.h>//close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/epoll.h>
#include <string>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"
#include <fcntl.h>
#include <iostream>

#define BUFFER	5
#define MAX_EVENTS 2
#define FAIL -1

class Client;
class Channel;

class Server {
private:
	int _fd, _port;
	std::string _passwd;
	struct sockaddr_in _address;
	struct epoll_event _ev, _events[MAX_EVENTS];
	std::map <int, Client*> _clients;
	std::map <std::string, Channel*> _channels;
	Server();

public:
	Server(int, std::string);
	~Server();
	Server(Server const &ref);
	Server &operator=(Server const &ref);

	int getFd() const;
	int getPort() const;
	Client *getClient(int);
	Channel *getChannel(std::string);

	void init();
	void run();
	void broadcast(Client*, std::string);
};