#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "webserv.hpp"

class Socket {
public:
			Socket();
			~Socket();
	void	bindSocket(int port);
	void	startListening();
	void	healthCheck();

private:
	struct sockaddr_in	m_address;
	int					m_fileDescriptor;
	int					m_status;

	class	SocketException : public std::exception {
		public:
			SocketException(const std::string& errorMessage);
			~SocketException() throw();
			const char* what() const throw();
		private:
			const std::string	m_errorMessage;
	};

};

#endif
