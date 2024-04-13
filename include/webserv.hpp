#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <cstring>
# include <cerrno>
# include <cstdlib>
# include <exception>
# include <vector>
# include <map>

# include <sys/socket.h>
# include <sys/errno.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/epoll.h>
# include <fcntl.h>
# include <sys/types.h>
# include <netdb.h>

namespace gnl {
	const int BUFFER_SIZE = 1024;
}

namespace sockets {
	const int IPV4 = AF_INET;
	const int TCP = SOCK_STREAM;
	const int DEFAULT_PROTOCOL = 0;
	const int MAX_CONNECTIONS_QUEUE = 100;
};

namespace epoll {
	const int MAX_EVENTS = 256;
	const int CAN_READ = EPOLLIN;
	const int CAN_WRITE = EPOLLOUT;

	enum EventType {
		UNKNOWN,
		NEW_CONNECTION,
		READ_OPERATIONS,
		WRITE_OPERATIONS
	};
};

namespace http {
	const char OK_200[] = "200 OK";
	const char REDIRECT_302[] = "302 REDIRECT";
	const char NOT_FOUND_404[] = "404 Not Found";
	const char NOT_IMPLEMENTED_501[] = "501 NOTIMPLEMENTED";
};

# include "../src/utils/CircularBuffer.hpp"
# include "../src/utils/utills.hpp"

# include "../src/classes/ServerSocket/ServerSocket.hpp"
# include "../src/classes/FileDescriptor/FileDescriptor.hpp"
# include "../src/classes/EventPoll/EventPoll.hpp"
# include "../src/classes/HttpRequest/HttpRequest.hpp"
# include "../src/classes/Clients/Clients.hpp"


void					fatalError(int _errno, void (*atExit)(void *) = NULL, 
							void* trash = NULL);
std::string				*getNextLine(const FileDescriptor& fd);

#endif
