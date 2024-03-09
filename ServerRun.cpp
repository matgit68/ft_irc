#include "Server.hpp"

void Server::init() {
	int opt = 1;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == FAIL) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	
	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) == FAIL)	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Listening on port " <<  port << std::endl;
	
	if (listen(fd, 3)  == FAIL)	{
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
	int valread, epollfd, nfds, newFd, addrlen = sizeof(address);
	char buf[BUFFER + 1];
	std::string tmp;

	if ((epollfd = epoll_create1(0)) == FAIL) { // creating the epoll listening fd (fd 4)
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == FAIL) {
		perror("epoll_ctl: fd");
		exit(EXIT_FAILURE);
	}
	while(true) {
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1); // number of fds that triggered epoll
		if (nfds == FAIL) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}
		for (int n = 0; n < nfds; ++n) { // going through all active fds
			if (events[n].data.fd == fd) { // if active fd is server fd, a new connection is incoming
				newFd = accept(fd, (struct sockaddr *) &address, (socklen_t*) &addrlen); // new connection will be reachable on newFd
				if (newFd == FAIL) {
					perror("accept");
					exit(EXIT_FAILURE);
				}
				setnonblocking(newFd);
				// fcntl(newFd, F_SETFL, O_NONBLOCK);
				ev.events = EPOLLIN;
				ev.data.fd = newFd;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newFd, &ev) == FAIL) { // adds newFd to the list of fd under surveillance
					perror("epoll_ctl: newFd");
					exit(EXIT_FAILURE);
				}
				if (clients.find(newFd) == clients.end()) { // creates a new Client and adds it to clients map
					std::cout << "Creating client for fd " << newFd << std::endl;
					clients[newFd] = new Client(newFd);
				}
			}
			else { // if active fd is NOT server fd, we're receving a message
				std::cout << "\n(Receiving on fd " << events[n].data.fd << ')' << std::endl;
				valread = recv(events[n].data.fd, buf, BUFFER, 0);
				if (valread == FAIL) {
					perror("recv");
					// exit(EXIT_FAILURE);
				}
				if (valread == 0) { // client disconnected -> remove fd from epoll, delete Client from clients map then close fd
					if (epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev) == FAIL) { // unwatch fd
						perror("epoll_ctl: newFd");
						exit(EXIT_FAILURE);
					}
					else {
						if (clients.find(events[n].data.fd) != clients.end()) // remove Client from map
							clients.erase(events[n].data.fd);
						else
							std::cout << "Couldnt find client of fd " << events[n].data.fd << std::endl;
						if (close(events[n].data.fd) == FAIL) // close fd
							std::cerr << "couldnt close fd " << events[n].data.fd << std::endl;
						std::cout << "Client " << events[n].data.fd << " disconnected" << std::endl;
					}
					continue;
				}
				buf[valread] = '\0';
				clients[events[n].data.fd]->receive(this, buf); // message will be buffered and treated by client instance
			}
		}
	}
}

void Server::broadcast(Client* client, std::string msg) {
	std::map<int, Client*>::iterator it;
	for (it = clients.begin(); it!=clients.end(); ++it)
    	if (it->second != client)
			if (send(it->second->getFd(), msg.c_str(), msg.size(), 0) != (ssize_t) msg.size())
				std::cerr << "Error sending msg" << std::endl;
}