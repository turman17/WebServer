#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "stdlibraries.hpp"

# include "../ServerBlock/ServerBlock.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"

class Clients;

namespace http {

	const char OK_200[] = "200 OK";
	const char REDIRECT_302[] = "302 Redirect";
	const char NOT_FOUND_404[] = "404 Not Found";
	const char NOT_IMPLEMENTED_501[] = "501 Not Implemented";
	const char CONTENT_TOO_LARGE_413[] = "413 Content Too Large";

	enum RequestStatus {
		REQUEST_RECEIVED,
		FILE_FOUND,
		FILE_NOT_FOUND,
		NOT_IMPLEMENTED,
		FILE_READ,
		RESPONSE_READY,
		RESPONSE_SENT,
		CLOSED
	};


	class HttpRequest {
	public:
								HttpRequest(const FileDescriptor& targetSocketFileDescriptor);
								~HttpRequest();
		bool					readRequest();
		http::RequestStatus		performReadOperations(const std::vector<ServerBlock>& serverBlocks,
									Clients& clients);
		RequestStatus			sendResponse();
		FileDescriptor			findFile();
		void					assignSettings(const std::vector<ServerBlock>& serverBlocks);
		void					setHostname(const std::string& hostname);
		void					setPort(const int& port);
		void					setDomain(const std::string& domain);
		void					setRequestStatus(const RequestStatus& status);
		const RequestStatus&	getRequestStatus();
	private:
		std::string							m_hostname;
		int									m_port;
		std::string							m_domain;
		std::string							m_requestMethod;
		std::string							m_fileURI;
		std::string							m_messageBody;
		std::string							m_contentType;
		std::string							m_contentLength;
		std::string							m_response;
		std::string							m_statusCode;
		std::map<std::string, std::string>	m_errorPages;
		int									m_maxBodySize;
		LocationBlock						m_settings;
		enum http::RequestStatus			m_requestStatus;
		const FileDescriptor				m_targetSocketFileDescriptor;

		
		//* Private methods
		HttpRequest();

		//* Exceptions

		class CloseConnection : public std::exception {
		public:
			const char* what() const throw();
		};
	};

	typedef std::pair<const FileDescriptor, HttpRequest*> FdReqPair;
	typedef std::map<const FileDescriptor, HttpRequest*> FdReqMap;
	typedef std::map<const FileDescriptor, HttpRequest*>::iterator Iterator;

};


#endif
