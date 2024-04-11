#include "webserv.hpp"

int	main(void) {

	try {
		ServerSocket	serverSocket("localhost", 8080);
		int fd = accept(serverSocket.getFileDescriptor(), NULL, NULL);
		char buffer[1024];
		read (fd, buffer, sizeof(buffer));
		std::cout << buffer << std::endl;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
}
