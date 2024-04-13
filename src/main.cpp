#include "webserv.hpp"


int	main(void) {

	try {
		Clients			clients;
		FileDescriptor	newConnection;
		ServerSocket	serverSocket("localhost", 8081);
		EventPoll		eventPoll(&serverSocket);
		while (true) {
			eventPoll.waitForEvents();
			std::cout << "New event" << std::endl;
			while (true) {
				try {
					int eventType = epoll::UNKNOWN;
					FileDescriptor newEvent = eventPoll.getNextEvent(eventType);
					std::cout << newEvent << std::endl;
					switch (eventType) {
						case epoll::NEW_CONNECTION:
							std::cout << "NEW CONNECTION" << std::endl;
							newConnection = accept(newEvent, NULL, NULL);
							newConnection.setNonBlocking();
							clients.addToSocketMap(newConnection, new HttpRequest(newConnection));
							eventPoll.add(newConnection, CAN_READ);
							break;
						default:
							HttpRequest* request = clients[newEvent];
							request->readRequest();
					}
				}
				catch (...) {
					break;
				}
			}
		}

	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
}
