#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include "webserv.hpp"

class ServerSocket {
public:
			ServerSocket(int port);
			~ServerSocket();
	void	setListeningState();
	void	healthCheck();

private:
	struct sockaddr_in	m_address;
	int					m_fileDescriptor;
	int					m_status;

	ServerSocket();



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
