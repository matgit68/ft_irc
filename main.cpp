#include "hpp.hpp"

int main(int argc, char **argv) {
	std::string bot;

	if (argc != 3 && argc != 4) {
		std::cerr << "Usage : ./ircserv <port 1025~65535> <password> [-bot=<botname>]" <<std::endl ;
		return EXIT_FAILURE;
	}
	int p = atoi(argv[1]);
	if (p < 1024 || p > 65535) {
		std::cerr << "Please select a port between 1024 and 65535";
		return EXIT_FAILURE;
	}
	
	if (argc == 4) {
		bot = argv[3];
		if (bot.compare(0, 5, "-bot=") != 0) {
			std::cerr << "Usage : ./ircserv <port 1025~65535> <password> [-bot=<botname>]" <<std::endl ;
			return EXIT_FAILURE;
		}
		bot.erase(0, 5);
	}
	Server s(p, argv[2], bot);

	s.init();
	s.run();

	return EXIT_SUCCESS;
}