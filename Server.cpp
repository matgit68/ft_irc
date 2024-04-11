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
void Server::setHostname(std::string str) { _hostname = str; }

int Server::getFd() const { return _fd; }

int Server::getPort() const { return _port; }

std::string Server::getPasswd() const { return _passwd; }
std::string Server::getHostname() const { return _hostname; }
std::string Server::getCreatedTime(void) const { return _createdTime; }

std::map<std::string, Channel*> Server::getChannelMap(void) const { return _channels; }

Channel *Server::getChannel(std::string name) {
	if (_channels.find(name) == _channels.end())
		return NULL;
	return _channels[name];
}

Client *Server::getClient(int fd) {
	if (_clients.find(fd) == _clients.end())
		return NULL;
	return _clients[fd];
}

Client *Server::getClient(std::string target) const {
	std::map<int, Client*>::const_iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
		if (it->second->getNick() == target)
			return (it->second);
	return NULL;
}

funPtr Server::getCommand(std::string name) {
	if (_commands.find(name) == _commands.end())
		return NULL;
return _commands[name];
}

void Server::createChannel(std::string chanName, Client *creator) {
	if (_channels.find(chanName) != _channels.end())
		return;
	std::string s; // no use, just to call addMode method
	_channels[chanName] = new Channel(this, chanName);
	_channels[chanName]->giveOp(creator->getFd());
	_channels[chanName]->addMode(creator, 't', s); // topic protected is the default mode
	_channels[chanName]->addClient(creator);
}

Channel * Server::addChannel(std::string channel) {
	if (_channels.find(channel) == _channels.end())
		_channels[channel] = new Channel(this, channel);
	return _channels[channel];
}


void Server::delChannel(std::string channel) {
	if (_channels.find(channel) != _channels.end())
		delete _channels[channel];
}

bool Server::findChannel(std::string channel)
{
	if(_channels.find(channel) == _channels.end())
		return false;
	return true;
}

void Server::dispChannels(Client *client) {
	//std::cout << "disp chan list" << std::endl;
	std::map<std::string, Channel*>::iterator it;
	for(it = _channels.begin(); it != _channels.end(); it++)
		ft_send(client->getFd(), it->second->getName());
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
	_commands["PART"] = &part;
}

void Server::sendRegistration(Client *client) {
	ft_send(client->getFd(), RPL_WELCOME(client));
	ft_send(client->getFd(), RPL_YOURHOST(client));
	ft_send(client->getFd(), RPL_CREATED(client, client->getServer()->getCreatedTime()));
	ft_send(client->getFd(), RPL_MYINFO(client, USERMODES, CHANMODES, ""));
	ft_send(client->getFd(), RPL_ISUPPORT(client, ISUPPORT));

	// client->setResponse(); // if we need to confirm the protocol
}

void Server::broadcast(Client* client, std::string msg) {
	std::map<int, Client*>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); it++)
		if (it->second != client)
			ft_send(it->first, msg);
}


bool Server::isNickAvailable(std::string& newNick) 
{
	std::map<int, Client*>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); it++)
	{
		if(it->second->getNick() == newNick)
			return false;
	}
	return true;
}

void Server::makeQuit(int fd) {
	ft_send(fd, "QUIT :SIGINT\r\n");
}

void Server::checkEmptyChannels() { // GPT version :)
    std::map<std::string, Channel*>::iterator it = _channels.begin();
    while (it != _channels.end()) {
        Channel *chan = it->second;
        if (chan->empty()) {
            std::cout << chan->getName() << " is empty. Deleting channel" << std::endl;
            delete chan;
            std::map<std::string, Channel*>::iterator erase_it = it;
            ++it; // Move to the next element before erasing the current one
            _channels.erase(erase_it); // Erase the current element
        } else {
            ++it; // Move to the next element
        }
    }
}
// void Server::checkEmptyChannels() {
// 	std::map<std::string, Channel*>::iterator it;
// 	Channel *chan;
// 	for (it = _channels.begin(); it != _channels.end(); it++) {
// 		chan = it->second;
// 		if (chan->empty()) {
// 			std::cout << chan->getName() << " is empty. Deleting channel" << std::endl;
// 			_channels.erase(chan->getName());
// 			delete chan;
// 		}
// 	}
// }

void Server::sendToClientsInTouch(Client *client, std::string msg, bool metoo) { // bool me = send to me too
	std::map<std::string, Channel*>::iterator it;
	std::set<int> dest;
	for (it = _channels.begin(); it != _channels.end(); it++) {
		if (it->second->isClient(client)) {
			std::set<int> tmp = it->second->getClientList();
			dest.insert(tmp.begin(), tmp.end());
		}
	}
	if (metoo)
		dest.insert(client->getFd());
	for (std::set<int>::iterator it = dest.begin(); it != dest.end(); it++)
		ft_send(*it, msg);
}


void Server::removeFromAllChannels(Client *client) {
    std::map<std::string, Channel*>::iterator it;
    std::string target = client->getNick(); 
    for (it = _channels.begin(); it != _channels.end(); it++) {
        Channel *chan = it->second; 
        if (chan->isClient(client)) { 
            chan->removeUser(client);
            std::cout << "Client " << target << " is removed from channel " << it->first << std::endl;
        }
    }
}

