#include "hpp.hpp"

std::string takeNextArg(std::string &str) { // single argument version uses ' ' as delimiter
	return takeNextArg(' ', str);
}

std::string takeNextArg(char sep, std::string &str) {
	std::string res;
	size_t space = str.find(sep);

	if (space == NPOS) {
		res = str;
		str.clear();
	}
	else {
		res = str.substr(0, space);
		str.erase(0, space + 1);
	}
	return res;
}

void trim(std::string &str) {
	str.erase(0, str.find_first_not_of(SPACES));
	str.erase(str.find_last_not_of(SPACES) + 1);
	for (size_t i = 0; i < str.size(); i++) {
		while (std::isspace(str[i]) && std::isspace(str[i + 1]))
			str.erase(i, 1);
	}
}