#pragma once

#include <string>

class Client{
private:
	std::string user, nick;
	int fd;
	Client(Client const &ref);
	Client &operator=(Client const &ref);

public:
	Client(std::string);
	~Client();
	int getFd() const;
	std::string getUser() const;
	std::string getNick() const;
	void setFd(int);
	void setNick(std::string);
};