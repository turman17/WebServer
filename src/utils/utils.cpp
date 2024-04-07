#include <iostream>
#include <cstdlib>


void	checkArguments(int argc) {
	if (argc != 2) {
		std::cerr << "Usage: ./webserv config" << std::endl;
		std::exit(1);
	}
}
