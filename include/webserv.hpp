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

	enum eventType {
		UNKNOWN,
		NEW_CONNECTION,
		//TODO
	};
};

# include "../src/utils/CircularBuffer.hpp"

# include "../src/classes/ServerSocket/ServerSocket.hpp"
# include "../src/classes/FileDescriptor/FileDescriptor.hpp"
# include "../src/classes/EventPoll/EventPoll.hpp"


void		fatalError(int _errno, void (*atExit)(void *) = NULL, void* trash = NULL);

#endif
