#include "hpp.hpp"
#include "Bot.hpp"

Server::Server(int p, std::string pass, std::string bot): _port(p), _passwd(pass), _hostname("localhost"),
														_createdTime(timestring()), _botname(bot) {
	char hostname[256];
    if (!gethostname(hostname, sizeof(hostname))) {
        _hostname.assign(hostname);
	}
	if (!_botname.empty()) {
		_keeper = new Bot(this);
		_keeperFd = 0;
	}
	initFunPtr();
}

Server::~Server() {
	std::map<int, Client*>::iterator it;
	std::map<std::string, Channel*>::iterator it2;
	for(it = _clients.begin(); it != _clients.end(); it++)
		delete it->second;
	_clients.clear();
	for(it2 = _channels.begin(); it2 != _channels.end(); it2++)
		delete it2->second;
	_channels.clear();
	if (!_botname.empty())
		delete _keeper;
}

Server::Server(Server const &ref) {
	*this = ref;
}

Server &Server::operator=(Server const &ref) {
	_fd = ref._fd;
	_port = ref._port;
	_passwd = ref._passwd;
	_hostname = ref._hostname;
	_clients = ref._clients;
	_channels = ref._channels;
	_commands = ref._commands;
	return *this;
}
void Server::setHostname(std::string str) { _hostname = str; }

int Server::getFd() const { return _fd; }

int Server::getPort() const { return _port; }

std::string Server::getPasswd() const { return _passwd; }
std::string Server::getHostname() const { return _hostname; }
std::string Server::getBotname() const {return _botname; }
int Server::getBotFd() const { return _keeper->getFd(); }
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
	if (!chanName.compare("#"))
		return ft_send(creator->getFd(), ERR_NOSUCHCHANNEL(creator, chanName));
	std::string s; // no use, just to call addMode method
	_channels[chanName] = new Channel(this, chanName);
	_channels[chanName]->giveOp(creator->getFd());
	_channels[chanName]->addMode(creator, 't', s); // topic protected is the default mode
	_channels[chanName]->addClient(creator);
	if (!_botname.empty()) {
		_keeper->join(chanName, creator->getNick());
		_channels[chanName]->giveOp(_keeper->getFd());
		ft_send(creator->getFd(), RPL_UMODEINCHANIS(_keeper, _channels[chanName], "+o", _keeper));
	}
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
	_commands["PART"] = &part;
	_commands["WHO"] = &who;
}

void Server::sendRegistration(Client *client) {
	ft_send(client->getFd(), RPL_WELCOME(client));
	ft_send(client->getFd(), RPL_YOURHOST(client));
	ft_send(client->getFd(), RPL_CREATED(client, client->getServer()->getCreatedTime()));
	ft_send(client->getFd(), RPL_MYINFO(client, USERMODES, CHANMODES, ""));
	ft_send(client->getFd(), RPL_ISUPPORT(client, ISUPPORT));
	client->setResponse(true);
}

void Server::delClient(int fd) {
	if (_clients.find(fd) != _clients.end()) {
		ft_send(fd, ERR_QUIT);
		if (close(fd) == FAIL) // close fd
			std::cerr << "Could not close fd " << fd << std::endl;
		else
			std::cout << RED "Client " << fd << " disconnected" RESET << std::endl;
		delete _clients[fd];
		_clients.erase(fd);
	}
}

bool Server::isNickAvailable(std::string& newNick) {
	std::map<int, Client*>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); it++)
	{
		if(it->second->getNick() == newNick)
			return false;
	}
	return true;
}

void Server::checkEmptyChannels() {
	std::map<std::string, Channel*>::iterator erase_it, it = _channels.begin();
	Channel *chan;

	while (it != _channels.end()) {
		chan = it->second;
		if (chan->empty()) {
			std::cout << "\t" << chan->getName() << " is empty. Deleting channel" RESET << std::endl;
			delete chan;
			erase_it = it;
			++it; // Move to the next element before erasing the current one
			_channels.erase(erase_it); // Erase the current element
		}
		else
			++it; // Move to the next element
	}
}

// bool = send to client too
void Server::sendToClientsInTouch(Client *client, std::string msg, bool metoo) { 
	std::set<int> dest;

	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		if (it->second->isClient(client)) {
			std::set<int> tmp = it->second->getClientList();
			dest.insert(tmp.begin(), tmp.end());
		}
	}
	dest.insert(client->getFd());
	if (!metoo)
		dest.erase(client->getFd());

	for (std::set<int>::iterator it = dest.begin(); it != dest.end(); it++)
		ft_send(*it, msg);
}

void Server::removeFromAllChannels(Client *client) {
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		Channel *chan = it->second;
		if (chan->isClient(client))
			chan->removeUser(client);
	}
}

void Server::ft_send(int fd, std::string msg) {
	if (msg.find("PING") == NPOS && msg.find("PONG") == NPOS) {
		if (!_botname.empty() && fd == _keeper->getFd())
			std::cout << YELLOW ">>(" << fd << ") : " BLUE << msg << RESET;
		else
			std::cout << YELLOW ">>(" << fd << ") : " RESET << msg;
	}
	if (!_botname.empty() && fd == _keeper->getFd())
		_keeper->react(msg);
	else if (_clients.find(fd) != _clients.end())
		send(fd, msg.c_str(), msg.size(), 0);
}