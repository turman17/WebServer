#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "stdlibraries.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"

class HttpRequest {
public:
			HttpRequest(const FileDescriptor& targetSocketFileDescriptor);
			~HttpRequest();
	bool	readRequest();
	bool	performRequest();
	bool	sendResponse();
private:
	std::string				m_requestMethod;
	std::string				m_fileURI;
	std::string				m_messageBody;
	std::string				m_contentType;
	std::string				m_contentLength;
	std::string				m_response;
	std::string				m_statusCode;
	const FileDescriptor	m_targetSocketFileDescriptor;
	//ServerBlockConfig		m_serverConfig;

	
	//* Private methods
	HttpRequest();
};

namespace http {
	const char OK_200[] = "200 OK";
	const char REDIRECT_302[] = "302 REDIRECT";
	const char NOT_FOUND_404[] = "404 Not Found";
	const char NOT_IMPLEMENTED_501[] = "501 NOTIMPLEMENTED";

	typedef std::pair<const FileDescriptor, HttpRequest*> FdReqPair;
	typedef std::map<const FileDescriptor, HttpRequest*> FdReqMap;
	typedef std::map<const FileDescriptor, HttpRequest*>::iterator Iterator;
};

#endif
