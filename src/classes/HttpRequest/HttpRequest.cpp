#include "HTTPRequest.hpp"
#include "../Clients/Clients.hpp"

using namespace http;

HttpRequest::HttpRequest(const FileDescriptor& targetSocketFileDescriptor) :
	m_domain(""),
	m_version("HTTP/1.1"),
	m_requestMethod(""),
	m_URL(""),
	m_filePath(""),
	m_queryString(""),
	m_requestBody(""),
	m_responseBody (""),
	m_contentType(""),
	m_contentLength(""), 
	m_response(""),
	m_statusCode(""),
	m_maxBodySize(1024),
	m_requestStatus(REQUEST_RECEIVED),
	m_targetSocketFileDescriptor(targetSocketFileDescriptor) {}


HttpRequest::~HttpRequest() {}


/**
 * @brief Reads the HTTP request from the client
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

	std::string::iterator firstSpace = std_next(firstLine->begin(),
		firstLine->find_first_of(' '));
	m_requestMethod = std::string(firstLine->begin(), firstSpace);
	if (m_requestMethod != "GET" && m_requestMethod != "POST" 
			&& m_requestMethod != "DELETE") {
				m_statusCode = NOT_IMPLEMENTED_501;
				return (true);
			}
	
	std::string::iterator questionMark = std::find(std_next(firstSpace), firstLine->end(), '?');
	std::string::iterator secondSpace = std::find(std_next(firstSpace), firstLine->end(), ' ');
	if (questionMark != firstLine->end()) {
		m_URL = std::string(std_next(firstSpace), questionMark);
		m_queryString = std::string(std_next(questionMark), secondSpace);
	} else {
		m_URL = std::string(std_next(firstSpace), secondSpace);
	}
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
			}
			if (flag)
				m_requestBody += *tmp;
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
 * @brief Performs read operations for the client HTTP request
 * 
 * @param serverBlocks The server blocks of the server
 * 
 * @return The status of the request
 * 
*/
RequestStatus	HttpRequest::performReadOperations(const std::vector<ServerBlock>& serverBlocks) {


	if (m_requestStatus == REQUEST_RECEIVED) {
		if (!readRequest()) {
			return (http::CLOSED);
		} else if (m_statusCode == NOT_IMPLEMENTED_501) {
			buildErrorPage(NOT_IMPLEMENTED_501);
			return (http::ERROR);
		}
		assignSettings(serverBlocks);
		StrVector allowedMethods = m_settings.getAllowedMethods();
		if (static_cast<int>(m_requestBody.length()) > m_maxBodySize) {
			buildErrorPage(CONTENT_TOO_LARGE_413);
			return (http::ERROR);
		} else if (std::find(allowedMethods.begin(), allowedMethods.end(),
			m_requestMethod) == allowedMethods.end()) {
				buildErrorPage(http::FORBIDDEN_403);
				return (http::ERROR);
			}
		if (m_requestMethod == "GET") {
			if (m_settings.getRedirection().first != "" && m_settings.getRedirection().second != "") {
				m_statusCode = m_settings.getRedirection().first;
				m_filePath = m_settings.getRedirection().second;
			} else {
				m_filePath = m_settings.getRoot() + m_URL;
			}
			if (isDirectory(m_filePath)) {
				m_filePath += m_settings.getIndexFile();
				if (!isFile(m_filePath) && m_settings.getDirectoryListing()) {
					performDirectoryListing();
					return (http::FILE_READ);
				}
			}
			std::ifstream requestedFile(m_filePath.c_str());
			if (requestedFile.fail() || isDirectory(m_filePath)) {
				buildErrorPage(NOT_FOUND_404);
				return (http::ERROR);
			}
			ifstreamToString(requestedFile, m_responseBody);
			requestedFile.close();
			return (http::FILE_READ);
		}
	}
	return CLOSED;
}


/**
 * @brief Assigns the settings for the client HTTP request
 * 
 * @param serverBlocks The server blocks of the server
 * 
*/
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
			if (startsWith(routePath, m_URL) && routePath.length() > lastMatch.length()) {
				lastMatch = routePath;
				m_settings = *it;
			}
		}
	if (lastMatch == "") {
		m_settings.setRoot(selectedServerBlock->getRoot());
	}
	m_errorPages = selectedServerBlock->getErrorPages();
}


/**
 * @brief Builds an error page for the client HTTP request
 * 
 * @param errorCode The error code of the response
 * 
*/
void	HttpRequest::buildErrorPage(const std::string& errorCode) {

	std::ifstream errorFile((m_errorPages[errorCode]).c_str());
	
	m_filePath = m_errorPages[errorCode];
	ifstreamToString(errorFile, m_responseBody);
	errorFile.close();
	m_statusCode = errorCode;
}


/**
 * @brief Expands the status code of the response
 * 
 * @return The status code of the response in string format
 * 
*/
std::string	HttpRequest::expandStatusCode() {

	std::string	expandedCode;

	if (m_statusCode == "404") {
		expandedCode = "404 Not Found";
	} else if (m_statusCode == "413") {
		expandedCode = "413 Request Entity Too Large";
	} else if (m_statusCode == "501") {
		expandedCode = "501 Not Implemented";
	} else if (m_statusCode == "500") {
		expandedCode = "500 Internal Server Error";
	} else if (m_statusCode == "200") {
		expandedCode = "200 OK";
	} else if (m_statusCode == "302") {
		expandedCode = "302 Found";
	} else if (m_statusCode == "403") {
		expandedCode = "403 Forbidden";
	} else if (m_statusCode == "301") {
		expandedCode = "301 Moved Permanently";
	} else {
		return (m_statusCode);
	}
	return (expandedCode);
}


/**
 * @brief Expands the content type of the response
 * 
 * @return The content type of the response in string format
 * 
*/
std::string	HttpRequest::expandContentType() {

	std::string			expandedType;
	std::string			fileExtension;

	if (m_contentType != "") {
		return (m_contentType);
	}
	std::string::size_type dot = m_filePath.find_last_of('.');
	if (dot != std::string::npos) {
		fileExtension = m_filePath.substr(dot + 1);
	} 

	if (fileExtension == "html" || fileExtension == "htm") {
		expandedType = "text/html";
	} else if (fileExtension == "css") {
		expandedType = "text/css";
	} else if (fileExtension == "js") {
		expandedType = "text/javascript";
	} else if (fileExtension == "jpeg" || fileExtension == "jpg") {
		expandedType = "image/jpeg";
	} else if (fileExtension == "png") {
		expandedType = "image/png";
	} else if (fileExtension == "gif") {
		expandedType = "image/gif";
	} else if (fileExtension == "bmp") {
		expandedType = "image/bmp";
	} else if (fileExtension == "ico") {
		expandedType = "image/x-icon";
	} else if (fileExtension == "svg") {
		expandedType = "image/svg+xml";
	} else if (fileExtension == "json") {
		expandedType = "application/json";
	} else if (fileExtension == "pdf") {
		expandedType = "application/pdf";
	} else if (fileExtension == "txt") {
		expandedType = "text/plain";
	} else {
		expandedType = "application/octet-stream";
	}
	return (expandedType);
}


/**
 * @brief Expands the content length of the response
 * 
 * @return The content length of the response in string format
 * 
*/
std::string HttpRequest::expandContentLength() {

	std::stringstream	tmp;
	
	tmp << m_responseBody.length();
	m_contentLength = tmp.str();
	return (m_contentLength);
}


/**
 * @brief Creates a response body with Directory Listing
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
bool		HttpRequest::performDirectoryListing() {

	DIR*		directory;
	dirent*		entry = NULL;
	std::string	htmlBody = "";

	directory = opendir(m_filePath.c_str());
	if (!directory) {
		return (false);
	}
	htmlBody += "\t<ul>\n";
	do {
		entry = readdir(directory);
		if (!entry) {
			break;
		}
		std::string entryName = entry->d_name;
		std::string path = m_URL;
		if (*path.rbegin() != '/') {
        	path += '/';
    	}
		path += entryName;
		htmlBody +=	"\t\t<li>\n"
						"\t\t\t<a href=\"" + path + "\" >" + entryName + "</a>\n"
					"\t\t</li>\n";
	} while (entry);
	htmlBody += "\t</ul>\n";
	m_responseBody = basicHtml("Directory Listing", htmlBody);
	m_contentType = "text/html";
	return (true);
}


/**
 * @brief Sends the HTTP response over a socket to a client
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
RequestStatus	HttpRequest::sendResponse() {

	std::cout << "\nSending response" << std::endl;

	m_response +=	m_version + " " + expandStatusCode() + "\r\n"
					"Content-Type: " + expandContentType() + "\r\n"
					"Content-Length: " + expandContentLength() + "\r\n"
					"\r\n" + m_responseBody;
	
	write(m_targetSocketFileDescriptor, m_response.c_str(), m_response.length());
	return (RESPONSE_SENT);
}


//* Setters and Getters

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


//* Exceptions

const char* HttpRequest::CloseConnection::what() const throw() {

	return ("Close active connection ASAP");
}
