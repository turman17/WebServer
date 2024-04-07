#include "webserv.hpp"

int	PORT = 0;

int	main(int argc, char** argv) {
	checkArguments(argc);
	(void)argv;

	Socket serverSocket;

	try 
	{
		serverSocket.healthCheck(); 
	}
	catch (std::exception& e) 
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}
