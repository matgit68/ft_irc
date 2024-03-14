#include "Server.hpp"

std::string timestring(void)
{
	time_t		rawtime;
	struct tm	*timeinfo;
	char		buffer[64];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%c", timeinfo);
	return (static_cast<std::string>(buffer));
}