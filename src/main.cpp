#include "webserv.hpp"

int	main(void) {

	try {
		ServerSocket	serverSocket("localhost", 8080);
		int fd = accept(serverSocket.getFileDescriptor(), NULL, NULL);
		HttpRequest test(fd);
		test.readRequest();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
}
