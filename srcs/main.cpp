#include "webserv.hpp"

int	main(int argc, char** argv) {
	checkArguments(argc);
	std::cout << argv[1] << std::endl;
	return (0);
}
