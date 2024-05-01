#include "webserv.hpp"

using namespace epoll;

int main(void) {

	Server	server;

	server.loadConfig("webserv.conf");
	server.printSettings();
	server.run();

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
