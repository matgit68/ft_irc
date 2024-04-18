#include "hpp.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage : ./ircserv <port 1025~65535> <password>" <<std::endl ;
		return EXIT_FAILURE;
	}
	int p = atoi(argv[1]);
	if (p < 1024 || p > 65535) {
		std::cerr << "Please select a port between 1024 and 65535";
		return EXIT_FAILURE;
	}
	
	Server s(p, argv[2]);

	s.init();
	s.run();

	return EXIT_SUCCESS;
}