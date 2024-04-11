#include "webserv.hpp"

void	fatalError(int _errno, void (*atExit)(void *), void* trash) {
	std::cerr << "Error: " << strerror(_errno) << " (fatal)" << std::endl;
	if (atExit && trash)
		atExit(trash);
	std::exit(10);
}
