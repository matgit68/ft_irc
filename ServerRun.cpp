#include "Server.hpp"
#include "Bot.hpp"

int g_sig = 1;
static void handle_sigint(int sig) { (void) sig; g_sig = 0; }

void Server::init() {
	signal(SIGINT, handle_sigint);
	int opt = 1;
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Server fd:" << _fd << std::endl;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == FAIL) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	setnonblocking(_fd);
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

void Server::run() {
	int valread, nfds, newFd, addrlen = sizeof(_address);
	char buf[BUFFER + 1];
	std::string tmp;

	if ((_epollfd = epoll_create1(0)) == FAIL) { // creating the epoll listening fd (probably fd 4)
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
	_ev.events = EPOLLIN;
	_ev.data.fd = _fd;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _fd, &_ev) == FAIL) {
		perror("epoll_ctl: fd");
		exit(EXIT_FAILURE);
	}
	_keeper->connectToServ();
	while(g_sig) { // SIGINT
		nfds = epoll_wait(_epollfd, _events, MAX_EVENTS, 0); // number of fds that triggered epoll
		if (nfds == FAIL && g_sig) {
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
				if (_keeper->getFd() == 0)
					_keeper->setFd(newFd);
				std::cout << "_keeperFd " <<  _keeperFd << std::endl;
				_ev.events = EPOLLIN;
				_ev.data.fd = newFd;
				if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, newFd, &_ev) == FAIL) { // adds newFd to the list of fd under surveillance
					perror("epoll_ctl: newFd");
					exit(EXIT_FAILURE);
				}
				if (_clients.find(newFd) == _clients.end()) // creates a new Client and adds it to clients map
					_clients[newFd] = new Client(newFd, this);
				std::cout << GREEN "Client " << newFd << " connected" RESET << std::endl;
			}
			else { // if active fd is NOT server fd, we're receving a message
				valread = recv(_events[n].data.fd, buf, BUFFER, 0);
				if (valread == FAIL) {
					perror(RED "recv" RESET);
					continue;
				}
				if (valread == 0) { // client disconnected -> remove fd from epoll, delete Client from clients map then close fd
					disconnectClient(n);
					continue;
				}
				buf[valread] = '\0';
				_clients[_events[n].data.fd]->receive(buf); // message will be buffered and handled by client instance
			}
		}
	}
	std::cout << "\nLeaving " << VERSION << std::endl;
}

void Server::disconnectClient(int n) {
	if (_clients.find(_events[n].data.fd) != _clients.end()) { // delete Client and remove Client from map
		Client *client = _clients[_events[n].data.fd];
		if (!client->isGone()) {
			std::cout << "AutoQuit activated(" << _events[n].data.fd << ")" << std::endl;
			sendToClientsInTouch(client, RPL_QUIT(_clients[_events[n].data.fd], "Disconnected"), false);
			removeFromAllChannels(client);
			checkEmptyChannels();
		}
		delete _clients[_events[n].data.fd];
		_clients.erase(_events[n].data.fd);
	}
	else
		std::cerr << "Couldnt find client of fd " << _events[n].data.fd << std::endl;
	if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[n].data.fd, &_ev) == FAIL) { // unwatch fd
		perror("epoll_ctl: newFd");
		exit(EXIT_FAILURE);
	}
	if (close(_events[n].data.fd) == FAIL) // close fd
		std::cerr << "couldnt close fd " << _events[n].data.fd << std::endl;
	else
		std::cout << RED "Client " << _events[n].data.fd << " disconnected" RESET << std::endl;
}

