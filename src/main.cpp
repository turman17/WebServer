#include "webserv.hpp"

int	main(void) {

	try {
		ServerSocket	serverSocket("localhost", 8081);
		int fd = accept(serverSocket.getFileDescriptor(), NULL, NULL);
		char buffer[1024];
		int bytes_read = read (fd, buffer, sizeof(buffer));
		write(1, buffer, bytes_read);
		close(fd);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
}
