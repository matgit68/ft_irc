#pragma once
#include "hpp.hpp"

#define BOTNAME "KeeperBot"
class Bot: public Client {
private:
	struct sockaddr_in _serverAddr;
	std::set<std::string> _persistence;
	std::map<std::string, std::string> _owners;
	int _sockFd;

public:
	Bot(Server *);
	~Bot();

	void setFd(int);
	// int getFd() const;
	void connectToServ();
	void sendToServ(std::string);
	void react(std::string);
	void parse(std::string);
	void keep(std::string, std::string, bool);
	void op(std::string, std::string);
	std::string helpMsg();
};