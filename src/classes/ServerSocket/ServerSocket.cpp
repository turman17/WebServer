#include "ServerSocket.hpp"

ServerSocket::ServerSocket() {}


ServerSocket::~ServerSocket() {}


/**
 * @brief Constructs a new TCP socket and binds its connection to a port and a hostname
 *
 * @param hostname hostname on which to assign a connection 
 * @param port port on which to assign a connection
*/
ServerSocket::ServerSocket(const std::string& hostname, const int& port)
	: m_fileDescriptor(socket(IPV4, TCP, DEFAULT_PROTOCOL)) {

	if (m_fileDescriptor < 0)
		ServerSocketException("socket() " + std::string (strerror(errno)));

	try {
		in_addr_t IpAddress = resolveHostname(hostname);
		bindSocket(IpAddress, port);
		setListeningState();
	}
	catch (const std::string& errorStr) {
		close(m_fileDescriptor);
		throw ServerSocketException(errorStr);
	}
}


/**
 * @brief Resolves the given hostname to its corresponding IP address.
 * 
 * @param hostname The hostname to resolve.
 * @return The corresponding IP address in `in_addr_t` format
 * @throw If the hostname couldn't be resolved, it throws an exception of type std::string containing the error message
 */
in_addr_t	ServerSocket::resolveHostname(const std::string& hostname) {

	struct addrinfo hint, *result(0);

	std::memset(&hint, 0, sizeof(hint));
	hint.ai_family = IPV4;
	hint.ai_socktype = TCP;

	int status = getaddrinfo(hostname.c_str(), NULL, &hint, &result);
	if (status < 0 || !result) {
		if (result)
			freeaddrinfo(result);
		throw "resolveHostname() - " + std::string(gai_strerror(status));
	}

	in_addr_t ipAddress =
		reinterpret_cast<sockaddr_in*>(result->ai_addr)->sin_addr.s_addr;
	freeaddrinfo(result);

	return (ipAddress);
}


/**
 * @brief Binds a socket to a certain ipAddress and port
 * 
 * @param ipAddress 
 * @throw If the bind function couldn't be resolved, it throws an exception of type std::string containing the error message
 */
void	ServerSocket::bindSocket(in_addr_t ipAddress, int port) {

	sockaddr_in connectionConfig;

	std::memset(&connectionConfig, 0, sizeof(connectionConfig));
	connectionConfig.sin_family = IPV4;
	connectionConfig.sin_addr.s_addr = ipAddress;
	connectionConfig.sin_port = htons(port);

	if (bind(m_fileDescriptor, reinterpret_cast<sockaddr*>(&connectionConfig),
		sizeof(connectionConfig)) == -1)
			throw ("bindSocket() - " + std::string(strerror(errno)));
}


/**
 * @brief Sets a listening state on the server socket
 * 
 * @throw If the listen function failed, it throws an exception of type std::string containing the error message
 */
void	ServerSocket::setListeningState() {
	if (listen(m_fileDescriptor, MAX_CONNECTIONS_QUEUE) == -1)
		throw ("setListeningState() - " + std::string(strerror(errno)));
}


//* Getters


int		ServerSocket::getFileDescriptor() const {
	return (m_fileDescriptor);
}


//* Exceptions


ServerSocket::ServerSocketException::ServerSocketException
	(const std::string& errorMessage) : m_errorMessage(
		"SocketException: " + errorMessage) {}


const char* ServerSocket::ServerSocketException::what() const throw() {
	return (m_errorMessage.c_str());
}


ServerSocket::ServerSocketException::~ServerSocketException() throw() {}
