#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>
# include <cstring>
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
#include <sys/types.h>
#include <netdb.h>

# include "../src/classes/ServerSocket/ServerSocket.hpp"

// sockets
const int IPV4 = AF_INET;
const int TCP = SOCK_STREAM;
const int DEFAULT_PROTOCOL = 0;

// listen() backlog
const int MAX_CONNECTIONS_QUEUE = 100;

// epoll events
const int CAN_READ = EPOLLIN;
const int CAN_WRITE = EPOLLOUT;

void		fatalError(int _errno, void (*atExit)(void *) = NULL, void* trash = NULL);

#endif
