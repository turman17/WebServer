#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include "stdlibraries.hpp"

# include "../FileDescriptor/FileDescriptor.hpp"

namespace sockets {
	const int IPV4 = AF_INET;
	const int TCP = SOCK_STREAM;
	const int DEFAULT_PROTOCOL = 0;
	const int MAX_CONNECTIONS_QUEUE = 100;
};

class ServerSocket {

public:
	ServerSocket(const std::string& hostname, const int& port);
	~ServerSocket();
	void					closeSocket();
	const FileDescriptor&	getFileDescriptor() const;

private:
	const FileDescriptor m_fileDescriptor;


	//* Private Methods
				ServerSocket();
	in_addr_t	resolveHostname(const std::string& hostname);
	void		bindSocket(in_addr_t ipAddress, int port);
	void		setListeningState();


	//* Exceptions
	class	ServerSocketException : public std::exception {
		public:
			ServerSocketException(const std::string& errorMessage);
			~ServerSocketException() throw();
			const char* what() const throw();
		private:
			const std::string	m_errorMessage;
	};

};

#endif
