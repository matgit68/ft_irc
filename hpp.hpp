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
#include <fcntl.h>
#include <iostream>
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#define BUFFER 512
#define MAX_EVENTS 5
#define FAIL -1

class Client;

void ping(Client *client, std::string args);
void kick(Client *client, std::string args);
void invite(Client *client, std::string args);
void topic(Client *client, std::string args);
void mode(Client *client, std::string args);
void cap(Client *client, std::string args);
void pass(Client *client, std::string args);
void nick(Client *client, std::string args);
void user(Client *client, std::string args);
void quit(Client *client, std::string args);
void join(Client *client, std::string args);
void privmsg(Client *client, std::string args);