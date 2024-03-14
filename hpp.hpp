#pragma once

#define BUFFER 512
#define MAX_EVENTS 5
#define FAIL -1

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
#include <set>
#include <vector>
#include <fcntl.h>
#include <iostream>
#include <ctime>

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "ErrMessages.hpp"


class Client;

std::string timestring(void);
void ping(Client *, std::string);
void kick(Client *, std::string);
void invite(Client *, std::string);
void topic(Client *, std::string);
void mode(Client *, std::string);
void cap(Client *, std::string);
void pass(Client *, std::string);
void nick(Client *, std::string);
void user(Client *, std::string);
void quit(Client *, std::string);
void join(Client *, std::string);
void privmsg(Client *, std::string);
void broad(Client *, std::string);
void dispChanList(Client *, std::string);

void ft_send(Client *, std::string);
