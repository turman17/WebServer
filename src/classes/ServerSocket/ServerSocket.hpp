#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include "webserv.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"

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
