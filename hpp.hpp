#pragma once

#define BUFFER 512
#define MAX_EVENTS 5
#define FAIL -1
#define NPOS std::string::npos
#define VERSION "ft_irc 0.5"
#define USERMODES ""
#define CHANMODES "itklo"

#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/epoll.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <ctime>

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "ErrMessages.hpp"
#define NPOS std::string::npos

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
void part(Client *, std::string); 
void broad(Client *, std::string);
void dispChanList(Client *, std::string);

void ft_send(int, std::string);
bool is_valid(const std::string nickname);
std::string takeNextArg(std::string &);
std::string takeNextArg(char, std::string &);
