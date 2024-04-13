#include "HTTPRequest.hpp"

using namespace http;

HttpRequest::HttpRequest(const FileDescriptor& targetSocketFileDescriptor)
	: m_requestMethod(""), m_fileURI(""), m_messageBody(""),
		m_contentType(""), m_response(""), m_statusCode(""),
	 	m_targetSocketFileDescriptor(targetSocketFileDescriptor) {}


HttpRequest::~HttpRequest() {}

/**
 * @brief Reads the client HTTP request
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
bool	HttpRequest::readRequest() {

	std::string* firstLine(0);

	try {
		firstLine = getNextLine(m_targetSocketFileDescriptor);
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

	std::cout << m_requestMethod << std::endl;

	std::string::iterator secondSpaceIt = std_next(firstSpaceIt,
		firstLine->find_first_of(' '));
	m_fileURI = std::string(std_next(firstSpaceIt), secondSpaceIt);

	delete(firstLine);
	if (m_requestMethod != "POST") {
		return (true);
	}

	bool flag = false;
	for (std::string *tmp = getNextLine(m_targetSocketFileDescriptor); 
		tmp; tmp = getNextLine(m_targetSocketFileDescriptor)) {
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
bool	HttpRequest::createResponse() {

	m_response += ("HTTP/1.1 " + m_statusCode + "\r\n");

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

	return (true);
}


