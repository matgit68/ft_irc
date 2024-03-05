#include "Client.hpp"

Client::Client(std::string u): user(u) {}

Client::~Client() {}

int Client::getFd() const { return fd; }

std::string Client::getUser() const { return user; }

std::string Client::getNick() const { return nick; }

void Client::setFd(int f) { fd = f; }

void Client::setNick(std::string n) { nick = n; }