#include "Server.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage : ./ircserv <port> <password>" <<std::endl ;
		return EXIT_FAILURE;
	}
	Server s(atoi(argv[1]), argv[2]);

	s.init();
	s.run();

	return 0;
}