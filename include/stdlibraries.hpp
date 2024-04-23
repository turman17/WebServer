#ifndef STDLIBRARIES_HPP
# define STDLIBRARIES_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <cstring>
# include <cerrno>
# include <cstdlib>
# include <exception>
# include <vector>
# include <map>
# include <algorithm>
# include <iterator>
# include <numeric>

# include <sys/socket.h>
# include <sys/errno.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/epoll.h>
# include <fcntl.h>
# include <sys/types.h>
# include <netdb.h>

# include "../src/utils/CircularBuffer.hpp"
# include "../src/utils/utills.hpp"

typedef std::pair<std::string, std::string> StrPair;
typedef std::vector<std::string>			StrVector;


namespace gnl {
	const int BUFFER_SIZE = 1024;
}

class FileDescriptor;

void					fatalError(int _errno, void (*atExit)(void *) = NULL, 
							void* trash = NULL);
std::string*	getNextLine(const FileDescriptor& fd, char buffer[gnl::BUFFER_SIZE + 1]);

#endif
