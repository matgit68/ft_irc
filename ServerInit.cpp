#include "Server.hpp"

void Server::init() {
	int opt = 1;
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	
	if (bind(fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Listening on port " <<  port << std::endl;
	
	if (listen(fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	addrlen = sizeof(address);
	std::cout << "Waiting for connections ..." << std::endl;
}

static int setnonblocking(int sfd) {
	int flags, s;

	flags = fcntl(sfd, F_GETFL, 0);
	if (flags == -1)
	{
		perror("fcntl");
		return -1;
	}
	flags |= O_NONBLOCK;
	s = fcntl(sfd, F_SETFL, flags);
	if (s == -1)
	{
		perror("fcntl");
		return -1;
	}
	return 0;
}

void Server::run() {
	int valread;
	char buf[BUFFER + 1];

	if ((epollfd = epoll_create1(0)) == -1) {
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		perror("epoll_ctl: fd");
		exit(EXIT_FAILURE);
	}
	for (;;) {
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}
		for (int n = 0; n < nfds; ++n) {
			if (events[n].data.fd == fd) {
				conn_sock = accept(fd, (struct sockaddr *) &address, (socklen_t*) &addrlen);
				if (conn_sock == -1) {
					perror("accept");
					exit(EXIT_FAILURE);
				}
				setnonblocking(conn_sock);
				ev.events = EPOLLIN;
				ev.data.fd = conn_sock;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
					perror("epoll_ctl: conn_sock");
					exit(EXIT_FAILURE);
				}
			}
			else {
				valread = recv(events[n].data.fd, buf, BUFFER, 0);
				if (valread == -1) {
					perror("recv");
					exit(EXIT_FAILURE);
				}
				buf[valread] = '\0';
				std::cout << buf;
			}
		}
	}
}