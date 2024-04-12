#pragma once

#define GREEN "\e[0;32m"
#define RED "\e[0;31m"
#define YELLOW "\e[0;33m"
#define RESET "\e[0m"

#define BUFFER 512
#define FAIL -1
#define MAX_EVENTS 5
#define NPOS std::string::npos

#define CHANMODES "iklot"
#define ISUPPORT ""
#define USERMODES ""
#define VERSION "ft_irc 0.0.5"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h> 
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "ErrMessages.hpp"
#include "Server.hpp"

class Client;

// Function pointers
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
void part(Client *, std::string);
void who(Client *, std::string);
void broad(Client *, std::string);
void dispChanList(Client *, std::string);

// Utils functions
void handle_sigint(int);
bool is_valid(const std::string nickname);
std::string timestring(void);
std::string takeNextArg(std::string &);
std::string takeNextArg(char, std::string &);
std::string parseReason(std::string msg);