#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "webserv.hpp"

class HttpRequest {
public:
			HttpRequest(const FileDescriptor& targetSocketFileDescriptor);
			~HttpRequest();
	bool	readRequest();
	bool	createResponse();
	bool	sendResponse();
private:
	std::string				m_requestMethod;
	std::string				m_fileURI;
	std::string				m_messageBody;
	std::string				m_contentType;
	std::string				m_response;
	std::string				m_statusCode;
	const FileDescriptor	m_targetSocketFileDescriptor;

	
	//* Private methods
	HttpRequest();
};

#endif
