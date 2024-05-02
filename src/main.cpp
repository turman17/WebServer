#include "webserv.hpp"

using namespace epoll;

#include <csignal>
#include <unistd.h>

volatile sig_atomic_t keepRunning = 1;

void signalHandler(int signal_num)
{
	std::cout << "Interrupt signal (" << signal_num << ") received.\n";
	keepRunning = 0;
}

int main()
{
	struct sigaction act;
	act.sa_handler = signalHandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT, &act, NULL);

	Server server;

	server.loadConfig("webserv.conf");
	server.printSettings();

	while (keepRunning)
	{
		server.run();
		std::cout << "You killed me my sweethurt\n";
		sleep(1);
	}

	std::cout << "Exiting main loop." << std::endl;
	return 0;
}

// int	main(void) {

// 	try {
// 		HttpRequest* request;
// 		std::vector<ServerSocket*>	serverSockets;
// 		Clients			clients;
// 		FileDescriptor	newConnection;
// 		ServerSocket	serverSocket("localhost", 8081);
// 		serverSockets.push_back(&serverSocket);
// 		EventPoll		eventPoll(serverSockets);
// 		serverSockets.~vector();
// 		while (true) {
// 			eventPoll.waitForEvents();
// 			while (NEW_EVENTS) {
// 				try {
// 					Event newEvent = eventPoll.getNextEvent();
// 					std::cout << "New Event on File Descriptor: " << newEvent << std::endl;
// 					switch (newEvent.getEvents()) {
// 						case NEW_CONNECTION:
// 							std::cout << "\tNew Connection" << std::endl;
// 							newConnection = accept(newEvent.fd(), NULL, NULL);
// 							newConnection.setNonBlocking();
// 							clients.addToSocketMap(newConnection, new HttpRequest(newConnection));
// 							eventPoll.add(newConnection, CAN_READ);
// 							break;
// 						case READ_OPERATIONS:
// 							request = clients[newEvent.fd()];
// 							if (!request->readRequest()) {
// 								eventPoll.remove(newEvent.fd());
// 								clients.delFromSocketMap(newEvent.fd());
// 								delete request;
// 							}
// 							else
// 								eventPoll.mod(newEvent.fd(), CAN_WRITE);
// 							break;
// 						case WRITE_OPERATIONS:
// 							request = clients[newEvent.fd()];
// 							if (request->performRequest()) {
// 								request->sendResponse();
// 							}
// 							eventPoll.remove(newEvent.fd());
// 							clients.delFromSocketMap(newEvent.fd());
// 							delete request;
// 							break;
// 						default:
// 							break;
// 					}
// 				}
// 				catch (...) {
// 					break;
// 				}
// 			}
// 		}

// 	}
// 	catch (std::exception& e) {
// 		std::cerr << e.what() << std::endl;
// 	}

// }
