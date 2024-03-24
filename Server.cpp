#include "hpp.hpp"

Server::Server(int p, std::string pass): _port(p), _passwd(pass), _hostname("localhost"), _createdTime(timestring()) {
	char hostname[256];
    if (!gethostname(hostname, sizeof(hostname))) {
        _hostname.assign(hostname);
	}
	initFunPtr();
}

Server::~Server() {}

Server::Server(Server const &ref) {
	*this = ref;
}

Server &Server::operator=(Server const &ref) {
	_fd = ref._fd;
	_port = ref._port;
	_passwd = ref._passwd;
	_clients = ref._clients;
	_channels = ref._channels;
	return *this;
}

int Server::getFd() const { return _fd; }

int Server::getPort() const { return _port; }

std::string Server::getPasswd() const { return _passwd; }

std::string Server::getHostname() const { return _hostname; }

std::map<std::string, Channel*>	Server::getChannels()		{ return (_channels); }

void Server::setHostname(std::string str) { _hostname = str; }

Client *Server::getClient(int fd) {
	if (_clients.find(fd) == _clients.end())
		return NULL;
	return _clients[fd];
}

Client *Server::getClient(std::string target) const {
	std::map<int, Client*>::const_iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++) // SEGFAULT ?! _client.begin()
		if (it->second->getNick() == target)
			return (it->second);
	return NULL;
}

Channel *Server::getChannel(std::string name) {
	if (_channels.find(name) == _channels.end())
		return NULL;
	return _channels[name];
}

std::string Server::getCreatedTime(void) const { return _createdTime; }

funPtr Server::getCommand(std::string name) {
	if (_commands.find(name) == _commands.end())
		return NULL;
return _commands[name];
}

Channel * Server::addChannel(std::string channel) {
	if (_channels.find(channel) == _channels.end())
		_channels[channel] = new Channel(this, channel);
	return _channels[channel];
}

Channel * Server::addChannel(std::string channel, std::string key) {
	if (_channels.find(channel) == _channels.end())
		_channels[channel] = new Channel(this, channel, key);
	return _channels[channel];
}

void Server::delChannel(std::string channel) {
	if (_channels.find(channel) != _channels.end())
		delete _channels[channel];
}

void Server::initFunPtr() {
	_commands["PING"] = &ping;
	_commands["KICK"] = &kick;
	_commands["INVITE"] = &invite;
	_commands["TOPIC"] = &topic;
	_commands["MODE"] = &mode;
	_commands["CAP"] = &cap;
	_commands["PASS"] = &pass;
	_commands["NICK"] = &nick;
	_commands["USER"] = &user;
	_commands["QUIT"] = &quit;
	_commands["JOIN"] = &join;
	_commands["PRIVMSG"] = &privmsg;
	_commands["BROAD"] = &broad;
	_commands["DCL"] = &dispChanList; // debug
}

void Server::sendRegistration(Client *client) {
	ft_send(client->getFd(), RPL_WELCOME(client));
	ft_send(client->getFd(), RPL_YOURHOST(client));
	ft_send(client->getFd(), RPL_CREATED(client, client->getServer()->getCreatedTime()));
	ft_send(client->getFd(), RPL_MYINFO(client, USERMODES, CHANMODES, ""));
	ft_send(client->getFd(), RPL_ISUPPORT(client, ""));
	// client->setResponse(); // if we need to confirm the protocol
}

void Server::broadcast(Client* client, std::string msg) {
	std::map<int, Client*>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); it++)
		if (it->second != client)
			ft_send(it->first, msg);
}

void Server::dispChannels(Client *client) {
	//std::cout << "disp chan list" << std::endl;
	std::map<std::string, Channel*>::iterator it;
	for(it = _channels.begin(); it != _channels.end(); it++)
		ft_send(client->getFd(), it->second->getName());
}
bool Server::isNickAvailable(std::string& newNick) //Checking if the nickname has already taken
{
	std::map<int, Client*>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); it++)
	{
		if(it->second->getNick() == newNick)
			return false;
	}
	return true;
}

std::map<std::string, Channel*> Server::getChannelMap(void) const { return _channels; }

bool Server::findChannel(std::string channel)
{
	if(_channels.find(channel) == _channels.end())
		return false;
	return true;
}

bool Server::isClientExists(int fd)
{
	if(_clients.find(fd) == _clients.end())
		return false;
	return true;
}


