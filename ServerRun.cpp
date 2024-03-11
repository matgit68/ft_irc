#include "Server.hpp"

void Server::init() {
	int opt = 1;
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == FAIL) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
	
	if (bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) == FAIL)	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Listening on port " <<  _port << std::endl;
	
	if (listen(_fd, 3)  == FAIL)	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	std::cout << "Waiting for connections ..." << std::endl;
}

static int setnonblocking(int sfd) {
	int flags, s;
	flags = fcntl(sfd, F_GETFL, 0);
	if (flags == FAIL)
	{
		perror("fcntl");
		return FAIL;
	}
	flags |= O_NONBLOCK;
	s = fcntl(sfd, F_SETFL, flags);
	if (s == FAIL)
	{
		perror("fcntl");
		return FAIL;
	}
	return 0;
}

void Server::run() {
	int valread, epollfd, nfds, newFd, addrlen = sizeof(_address);
	char buf[BUFFER + 1];
	std::string tmp;

	if ((epollfd = epoll_create1(0)) == FAIL) { // creating the epoll listening fd (fd 4)
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
	_ev.events = EPOLLIN;
	_ev.data.fd = _fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, _fd, &_ev) == FAIL) {
		perror("epoll_ctl: fd");
		exit(EXIT_FAILURE);
	}
	while(true) {
		nfds = epoll_wait(epollfd, _events, MAX_EVENTS, -1); // number of fds that triggered epoll
		if (nfds == FAIL) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}
		for (int n = 0; n < nfds; ++n) { // going through all active fds
			if (_events[n].data.fd == _fd) { // if active fd is server fd, a new connection is incoming
				newFd = accept(_fd, (struct sockaddr *) &_address, (socklen_t*) &addrlen); // new connection will be reachable on newFd
				if (newFd == FAIL) {
					perror("accept");
					exit(EXIT_FAILURE);
				}
				setnonblocking(newFd);
				// fcntl(newFd, F_SETFL, O_NONBLOCK);
				_ev.events = EPOLLIN;
				_ev.data.fd = newFd;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newFd, &_ev) == FAIL) { // adds newFd to the list of fd under surveillance
					perror("epoll_ctl: newFd");
					exit(EXIT_FAILURE);
				}
				if (_clients.find(newFd) == _clients.end()) { // creates a new Client and adds it to clients map
					std::cout << "Creating client for fd " << newFd << std::endl;
					_clients[newFd] = new Client(newFd, this);
				}
			}
			else { // if active fd is NOT server fd, we're receving a message
				std::cout << "\n(Receiving on fd " << _events[n].data.fd << ')' << std::endl;
				valread = recv(_events[n].data.fd, buf, BUFFER, 0);
				if (valread == FAIL) {
					perror("recv");
					// exit(EXIT_FAILURE);
				}
				if (valread == 0) { // client disconnected -> remove fd from epoll, delete Client from clients map then close fd
					if (epoll_ctl(epollfd, EPOLL_CTL_DEL, _events[n].data.fd, &_ev) == FAIL) { // unwatch fd
						perror("epoll_ctl: newFd");
						exit(EXIT_FAILURE);
					}
					else {
						if (_clients.find(_events[n].data.fd) != _clients.end()) // remove Client from map
							_clients.erase(_events[n].data.fd);
						else
							std::cerr << "Couldnt find client of fd " << _events[n].data.fd << std::endl;
						if (close(_events[n].data.fd) == FAIL) // close fd
							std::cerr << "couldnt close fd " << _events[n].data.fd << std::endl;
						std::cout << "Client " << _events[n].data.fd << " disconnected" << std::endl;
					}
					continue;
				}
				buf[valread] = '\0';
				_clients[_events[n].data.fd]->receive(buf); // message will be buffered and treated by client instance
			}
		}
	}
	std::cout << "boucle" << std::endl;
}

void Server::broadcast(Client* client, std::string msg) {
	std::map<int, Client*>::iterator it;
std::cerr << "debug BROADCAST : " + msg;
	if (!client->getStatus() && !msg.compare(0, 6, "CAP LS") && (!msg[6] || msg[6] == ' '))
	{
		std::string cap = "CAP * LS :\r\n";
std::cerr << "debug CAP LS : " + cap;
		if (send(client->getFd(), cap.c_str(), cap.size(), 0)!= (ssize_t) cap.size())
			std::cerr << "Error sending msg" << std::endl;
	}
	else if (!msg.compare("CAP END\r\n"))
	{
		if (!client->getStatus())
		{
			usleep(100);
			std::string end = client->getUser() + " " + client->getNick() + " :Welcome to the Internet Relay Network " + client->getNick() + "\r\n";
std::cerr << "debug CAP END : " + end;
			if (send(client->getFd(), end.c_str(), end.size(), 0) != (ssize_t) end.size())
				std::cerr << "Error sending msg" << std::endl;
			std::string wel = client->getNick() + " :Welcome to the <networkname> Network, " + client->getPrefix() + "\r\n";
std::cerr << "debug CAP END : " + wel;
			if (send(client->getFd(), wel.c_str(), wel.size(), 0) != (ssize_t) wel.size())
				std::cerr << "Error sending msg" << std::endl;
		}
	}
	else if (!client->getStatus() && !msg.compare(0, 5, "USER "))
	{
		// to parse and set
		// USER <username> <hostname> <servername> :<realname>
std::cerr << "debug USER : " + msg;
		msg.erase(0, 4);
		msg.erase(0, msg.find_first_not_of(" \r\t\v\f\n", 0));
std::cerr << "debug USER : " + msg;
		client->setUser(msg.substr(0, msg.find_first_of(" \r\t\v\f\n", 0)));	
						// ERR_NEEDMOREPARAMS if empty 
						// MUST NOT contain any of the following characters: space (' ', 0x20), 
							// comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), 
							// exclamation mark ('!', 0x21), at sign ('@', 0x40).
						// MUST NOT start with any of the following characters: dollar ('$', 0x24), 
							// colon (':', 0x3A).
						// MUST NOT start with a character listed as a channel type, channel membership
							// prefix, or prefix listed in the IRCv3 multi-prefix Extension.
						// SHOULD NOT contain any dot character ('.', 0x2E).
		msg.erase(0, msg.find_first_not_of(" \r\t\v\f\n", 0));
		msg.erase(0, msg.find_first_of(" \r\t\v\f\n", 0));
std::cerr << "debug USER : " + msg;
		client->setHost(msg.substr(0, msg.find_first_of(" \r\t\v\f\n", 0)));
		msg.erase(0, msg.find_first_of(":", 0) + 1);
std::cerr << "debug USER : " + msg;
		client->setReal(msg);
	}
	else if (!client->getStatus() && !msg.compare(0, 5, "NICK "))
	{
		msg.erase(0, 4);
		msg.erase(0, msg.find_first_not_of(" \r\t\v\f\n", 0));
std::cerr << "debug NICK : " + msg;
		client->setNick(msg);
	}
	else
		for (it = _clients.begin(); it != _clients.end(); ++it)
			if (it->second != client)
				if (send(it->second->getFd(), msg.c_str(), msg.size(), 0) != (ssize_t) msg.size())
					std::cerr << "Error sending msg" << std::endl;
}