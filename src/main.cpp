#include "webserv.hpp"

int	PORT = 0;

int	main(int argc, char** argv) {
	checkArguments(argc);
	(void)argv;

	ServerSocket serverSocket(PORT);

	serverSocket.setListeningState();

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
