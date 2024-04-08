#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <cstdlib>
# include <exception>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/errno.h>
# include <stdio.h>

# include "../src/classes/ServerSocket/ServerSocket.hpp"

const int IPV4 = AF_INET;
const int TCP = SOCK_STREAM;
const int DEFAULT_PROTOCOL = 0;

void	checkArguments(int argc);

#endif
