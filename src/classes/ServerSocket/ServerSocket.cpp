#include "ServerSocket.hpp"

// @brief Constructs a new TCP socket and binds its connection to a port
// @param port the port on which to assign a connection
ServerSocket::ServerSocket(int port) : m_fileDescriptor(socket(IPV4, TCP, DEFAULT_PROTOCOL)), m_status(0)
{
	m_status = errno;
	std::memset(&m_address, 0, sizeof(m_address));
	if (m_status != 0)
		return ;

	m_address.sin_family = IPV4;
	m_address.sin_addr.s_addr = htonl(INADDR_ANY);
	m_address.sin_port = htons(port);

	bind(m_fileDescriptor, reinterpret_cast<sockaddr*>(&m_address), sizeof(m_address));
	m_status = errno;
}

ServerSocket::ServerSocket() {}

ServerSocket::~ServerSocket() {}

// @brief Set a listening state to the socket in order to receive requests 
void	ServerSocket::setListeningState() {
	if (m_status == 0)
		return ;
	listen(m_fileDescriptor, 10);
	m_status = errno;
}

// @brief Run an health test after using socket functions
void	ServerSocket::healthCheck(){
	if (m_status != 0)
		throw ServerSocketException(strerror(m_status));
	std::cout << "Socket Health Check ✅" << std::endl;
}

ServerSocket::ServerSocketException::ServerSocketException(const std::string& errorMessage) :
	m_errorMessage("SocketException: " + errorMessage) {}

const char* ServerSocket::ServerSocketException::what() const throw() {
	return (m_errorMessage.c_str());
}

ServerSocket::ServerSocketException::~ServerSocketException() throw() {}
