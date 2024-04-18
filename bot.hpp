#pragma once
#include "hpp.hpp"

class Server;
class Client;
class Server;

class Bot: public Client{
private:
	std::set<std::string> _persistents;

public:
	Bot();
	~Bot();

	void addChannel();
};