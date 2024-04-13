#include "HTTPRequest.hpp"

using namespace http;

HttpRequest::HttpRequest(const FileDescriptor& targetSocketFileDescriptor)
	: m_requestMethod(""), m_fileURI(""), m_messageBody(""),
		m_contentType(""), m_contentLength(""), m_response(""), m_statusCode(""),
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

	// TODO Look for requested file

	if (m_requestMethod != "POST") {
		return (true);
	}

	bool flag = false;
	for (std::string *tmp = getNextLine(m_targetSocketFileDescriptor, gnlBuffer); 
		tmp; tmp = getNextLine(m_targetSocketFileDescriptor, gnlBuffer)) {
		if (flag)
			m_messageBody += *tmp;
		else if (*tmp == "\r\n")
			flag = true;
		delete(tmp);
	}
	return (true);
}


/**
 * @brief Creates a response for the client HTTP request
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
bool	HttpRequest::performRequest() {

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
	
	//? Connection: close

	return (true);
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


