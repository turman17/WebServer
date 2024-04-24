#include "HTTPRequest.hpp"

using namespace http;

HttpRequest::HttpRequest(const FileDescriptor& targetSocketFileDescriptor) :
	m_requestMethod(""),
	m_fileURI(""),
	m_messageBody(""),
	m_contentType(""),
	m_contentLength(""), 
	m_response(""),
	m_statusCode(""),
	m_requestStatus(REQUEST_RECEIVED),
	m_targetSocketFileDescriptor(targetSocketFileDescriptor) {}


HttpRequest::~HttpRequest() {}


/**
 * @brief Reads the client HTTP request
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
bool	HttpRequest::readRequest() {

	std::string*	firstLine(0);
	char			gnlBuffer[gnl::BUFFER_SIZE + 1] = {0};

	try {
		firstLine = getNextLine(m_targetSocketFileDescriptor, gnlBuffer);
		if (!firstLine) {
			throw BadRead();
		}
	}
	catch (const std::exception& e) {
		return (false);
	}

	std::string::iterator firstSpaceIt = std_next(firstLine->begin(),
		firstLine->find_first_of(' '));
	m_requestMethod = std::string(firstLine->begin(), firstSpaceIt);
	if (m_requestMethod != "GET" && m_requestMethod != "POST" 
			&& m_requestMethod != "DELETE") {
				m_statusCode = NOT_IMPLEMENTED_501;
				return (true);
			}

	std::string::iterator secondSpaceIt = std_next(firstSpaceIt,
		firstLine->find_first_of(' '));
	m_fileURI = std::string(std_next(firstSpaceIt), secondSpaceIt);

	delete(firstLine);

	try {
		bool flag = false;
		for (std::string *tmp = getNextLine(m_targetSocketFileDescriptor, gnlBuffer); 
			tmp; tmp = getNextLine(m_targetSocketFileDescriptor, gnlBuffer)) {
			if (!flag && startsWith("Host:", *tmp)) {
				m_domain = std::string(std_next(tmp->begin(), 6), tmp->end());
			}
			if (flag)
				m_messageBody += *tmp;
			else if (*tmp == "\r\n")
				flag = true;
			delete(tmp);
		}
	}
	catch (const std::exception& e) {
		return (false);
	}
	return (true);
}


/**
 * @brief Creates a response for the client HTTP request
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
bool	HttpRequest::performRequest(const std::vector<ServerBlock>& serverBlocks, Clients& clients) {

	if (m_requestStatus == REQUEST_RECEIVED) {
		if (!readRequest()) {
			throw CloseConnection();
		}
		assignSettings(serverBlocks);
		int fd = findFile();
		if (fd != -1) {
			clients.addToFilesMap(fd, this);
			m_requestStatus = REQUEST_READ;
		}
		else {
			m_requestStatus = FILE_NOT_FOUND;
		}
	}
	else if (m_requestStatus == REQUEST_READ) {
		if (m_messageBody.length() > m_maxBodySize) {
			//* Set error body too big
		}
	}








	// std::stringstream	tmp;

	// m_messageBody = "<!DOCTYPE html>\r\n"
	// 				"<html>\r\n"
	// 				"<head>\r\n"
	// 					"<title>Test Page</title>\r\n"
	// 				"</head>\r\n"
	// 				"<body>\r\n"
	// 					"<h1>Hello, World!</h1>\r\n"
	// 				"</body>\r\n"
	// 				"</html>\r\n";
	
	// tmp << m_messageBody.length();

	// m_contentLength = tmp.str();

	// m_response +=	"HTTP/1.1 200 OK\r\n"
	// 				"Content-Type: text/html\r\n"
	// 				"Content-Length: " + m_contentLength + "\r\n"
	// 				"\r\n";

	// m_response += m_messageBody;
	
	// //? Connection: close

	// return (true);
}


void	HttpRequest::assignSettings(const std::vector<ServerBlock>& serverBlocks) {

	const ServerBlock*	defaultServerBlock = NULL;
	const ServerBlock*	selectedServerBlock = NULL;
	int				i = 0;

	for (std::vector<ServerBlock>::const_iterator it = serverBlocks.begin();
		it != serverBlocks.end(); std::advance(it, 1)) {

			if (m_port != (*it).getPort()
				|| (normalizeHostname((*it).getHostname()) != "0.0.0.0" 
				&& normalizeHostname(m_hostname) != normalizeHostname((*it).getHostname()))) {
					continue;
			}
			if (i == 0) {
				defaultServerBlock = &(*it);
				i++;
			}
			if (m_domain == (*it).getServerName()) {
				selectedServerBlock = &(*it);
				break;
			}
		}
	if (!selectedServerBlock) {
		selectedServerBlock = defaultServerBlock;
	}

	std::string lastMatch = "";
	for (std::vector<LocationBlock>::const_iterator it = selectedServerBlock->getLocationBlocks().begin();
		it != selectedServerBlock->getLocationBlocks().end(); std::advance(it, 1)) {

			std::string routePath = (*it).getRoutePath();
			if (startsWith(routePath, m_fileURI) && routePath.length() > lastMatch.length()) {
				lastMatch = routePath;
				m_settings = *it;
			}
		}
	if (lastMatch == "") {
		m_settings.setRoot(selectedServerBlock->getRoot());
	}
	m_errorPages = selectedServerBlock->getErrorPages();
}


FileDescriptor	HttpRequest::findFile() {

	std::string	file = m_settings.getRoot() + m_fileURI;

	int	fd = open(file.c_str(), O_RDONLY);
	if (fd == -1) {
		m_statusCode = NOT_FOUND_404;
	}
	return (fd);
}




/**
 * @brief Sends the HTTP response over a socket to a client
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
bool	HttpRequest::sendResponse() {

	write(m_targetSocketFileDescriptor, m_response.c_str(), m_response.length());
	return (true);
}


void	HttpRequest::setHostname(const std::string& hostname) {
	m_hostname = hostname;
}


void	HttpRequest::setPort(const int& port) {
	m_port = port;
}


void	HttpRequest::setDomain(const std::string& domain) {
	m_domain = domain;
}


const char* HttpRequest::CloseConnection::what() const throw() {

	return ("Close connection ASAP");
}
