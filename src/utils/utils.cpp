#include "stdlibraries.hpp"

void	fatalError(int _errno, void (*atExit)(void *), void* trash) {
	std::cerr << "Error: " << strerror(_errno) << " (fatal)" << std::endl;
	if (atExit && trash)
		atExit(trash);
	std::exit(10);
}


bool	startsWith(const std::string& toExpect, const std::string& text) {

	std::stringstream	tmpsStream(text);
	std::string			tmp;

	tmpsStream >> tmp;
	if (tmp != toExpect) {
		return false;
	}
	return true;
}

unsigned int countWords(const std::string& str)
{
    std::stringstream stream(str);
    return std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
}

bool isOnlySpaces(const std::string& str) {

	std::size_t i = 0;
	std::size_t len = str.length();
	while (i < len) {
		if (!isspace(str[i])) {
			return false;
		}
		i++;
	}
	return true;
}


std::string normalizeHostname(const std::string& hostname) {
	if (hostname == "localhost" || hostname == "127.0.0.1") {
		return "localhost";
	}
	return hostname;
}

