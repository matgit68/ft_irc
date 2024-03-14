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
<<<<<<< HEAD
	
=======
	std::string getCreatedTime(void) const;
>>>>>>> 4a28f7aa2de2f57531a2b95d6520a0460f1ac100

	void initFunPtr();
	void init();
	void run();
	void broadcast(Client*, std::string);
<<<<<<< HEAD


=======
	void sendRegistration(Client *);
>>>>>>> 4a28f7aa2de2f57531a2b95d6520a0460f1ac100
	// void cap(Client *client, std::string args);
	// void invite(Client *client, std::string args);
	// void join(Client *client, std::string args);
	// void kick(Client *client, std::string args);
	// void mode(Client *client, std::string args);
	// void nick(Client *client, std::string args);
	// void pass(Client *client, std::string args);
	// void ping(Client *client, std::string args);
	// void privmsg(Client *client, std::string args);
	// void quit(Client *client, std::string args);
	// void topic(Client *client, std::string args);
	// void user(Client *client, std::string args);
};