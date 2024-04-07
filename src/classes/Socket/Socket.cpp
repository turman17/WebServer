#include "Socket.hpp"

// @brief Constructs a new TCP socket and initializes its file descriptor.
Socket::Socket() : m_fileDescriptor(socket(IPV4, TCP, DEFAULT_PROTOCOL)), m_status(0)
{
	m_status = errno;
	std::memset(&m_address, 0, sizeof(m_address));
}

Socket::~Socket() {}

// @brief Assings the socket to a connection on a certain port (use only on server sockets)
// @param port the port on which to assign a connection
void	Socket::bindSocket(int port) {
	if (m_status != 0)
		return ;

	m_address.sin_family = IPV4;
	m_address.sin_addr.s_addr = htonl(INADDR_ANY);
	m_address.sin_port = htons(port);

	bind(m_fileDescriptor, reinterpret_cast<sockaddr*>(&m_address), sizeof(m_address));
	m_status = errno;
}

// @brief Set a listening state to the socket in order to receive requests 
void	Socket::startListening() {
	if (m_status == 0)
		return ;
	listen(m_fileDescriptor, 10);
	m_status = errno;
}

// @brief Run an health test after using socket functions
void	Socket::healthCheck(){
	if (m_status != 0)
		throw SocketException(strerror(m_status));
	std::cout << "Socket Health Check ✅" << std::endl;
}

Socket::SocketException::SocketException(const std::string& errorMessage) :
	m_errorMessage("SocketException: " + errorMessage) {}

const char* Socket::SocketException::what() const throw() {
	return (m_errorMessage.c_str());
}

Socket::SocketException::~SocketException() throw() {}
