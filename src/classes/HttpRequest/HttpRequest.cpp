#include "HTTPRequest.hpp"
#include "../Clients/Clients.hpp"

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

	firstLine = getNextLine(m_targetSocketFileDescriptor, gnlBuffer);
	if (!firstLine) {
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
	std::string::iterator secondSpaceIt = std::find(++firstSpaceIt, firstLine->end(), ' ');
	m_fileURI = std::string(firstSpaceIt, secondSpaceIt);

	delete(firstLine);

	bool flag = false;
	while (true) {
		try {
			std::string* tmp = getNextLine(m_targetSocketFileDescriptor, gnlBuffer);
			if (!tmp) {
				return (false);
			}
			if (!flag && startsWith("Host:", *tmp)) {
				m_domain = std::string(std_next(tmp->begin(), 6),
					std_next(tmp->begin(), tmp->find_last_of(':')));
				std::cout << "Domain: " << m_domain << std::endl;
			}
			if (flag)
				m_messageBody += *tmp;
			else if (*tmp == "\r\n")
					flag = true;
			delete(tmp);
		}
		catch (const std::exception& e) {
			break;
		}
	}
	return (true);
}


/**
 * @brief Creates a response for the client HTTP request
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
RequestStatus	HttpRequest::performReadOperations(
		const std::vector<ServerBlock>& serverBlocks, Clients& clients) {

	if (m_requestStatus == REQUEST_RECEIVED) {
		if (!readRequest()) {
			std::cout << "Failed to read request" << std::endl;
			return (http::CLOSED);
		}
		if (m_statusCode == NOT_IMPLEMENTED_501) {
			return (http::NOT_IMPLEMENTED);
		}
		assignSettings(serverBlocks);
		FileDescriptor fd = findFile();
		if (fd == -1) {
			m_statusCode = NOT_FOUND_404;
			return (http::FILE_NOT_FOUND);
		}
		clients.addToFilesMap(fd, this);
		return (http::FILE_FOUND);
	}
	else if (m_requestStatus == FILE_FOUND) {
		if (static_cast<int>(m_messageBody.length()) > m_maxBodySize) {
			m_statusCode = CONTENT_TOO_LARGE_413;
		}
		else if (m_statusCode == NOT_FOUND_404 || m_statusCode == NOT_IMPLEMENTED_501) {
			;
		}
		else {
			;
		}
	}
	return CLOSED;

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

	std::cout << "File Path: " << file << std::endl;
	return (open(file.c_str(), O_RDONLY));
}


/**
 * @brief Sends the HTTP response over a socket to a client
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
RequestStatus	HttpRequest::sendResponse() {

	std::stringstream	tmp;

	m_messageBody = "<!DOCTYPE html>\r\n"
					"<html>\r\n"
					"<head>\r\n"
						"<title>Test Page</title>\r\n"
					"</head>\r\n"
					"<body>\r\n"
						"<h1>Hello, World!</h1>\r\n"
					"</body>\r\n"
					"</html>\r\n";
	
	tmp << m_messageBody.length();

	m_contentLength = tmp.str();

	m_response +=	"HTTP/1.1 200 OK\r\n"
					"Content-Type: text/html\r\n"
					"Content-Length: " + m_contentLength + "\r\n"
					"\r\n";

	m_response += m_messageBody;
	
	write(m_targetSocketFileDescriptor, m_response.c_str(), m_response.length());
	return (RESPONSE_SENT);
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


void	HttpRequest::setRequestStatus(const http::RequestStatus& status) {

	m_requestStatus = status;
}

const RequestStatus&	HttpRequest::getRequestStatus() {
	
	return (m_requestStatus);
}


const char* HttpRequest::CloseConnection::what() const throw() {

	return ("Close active connection ASAP");
}
