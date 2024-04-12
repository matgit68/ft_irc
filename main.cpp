#include "hpp.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage : ./ircserv <port> <password>" <<std::endl ;
		return EXIT_FAILURE;
	}
	int p = atoi(argv[1]);
	if (p < 1024 || p > 65535)
	{
		// changer port ou exit erreur
	}
	signal(SIGINT, handle_sigint);
	Server s(p, argv[2]);

	s.init();
	s.run();

	return 0;
}