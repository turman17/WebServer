#include "HttpRequest.hpp"
#include "../Clients/Clients.hpp"

using namespace http;

HttpRequest::HttpRequest(const FileDescriptor& targetSocketFileDescriptor) :
	m_domain(""),
	m_version("HTTP/1.1"),
	m_requestMethod(""),
	m_URL(""),
	m_filePath(""),
	m_queryString(""),
	m_responseBody (""),
	m_contentType(""),
	m_requestContentType(""),
	m_contentLength("0"), 
	m_response(""),
	m_statusCode(""),
	m_maxBodySize(1024),
	m_requestStatus(REQUEST_NOT_READ),
	m_parseState(FIRST_LINE),
	m_targetSocketFileDescriptor(targetSocketFileDescriptor) {}


HttpRequest::~HttpRequest() {}


/**
 * @brief Reads the HTTP request from the client
 * 
 * @return If everything goes well returns true and in case of an error returns false
 * 
*/
RequestStatus	HttpRequest::readRequest() {

	ssize_t				bytesRead = 0;
	unsigned int		offset = 0;
	char				buffer[BUFFER_SIZE + 1] = {0};
	std::vector<byte>*	line(0);

	bytesRead = read(m_targetSocketFileDescriptor, buffer, BUFFER_SIZE);
	if (bytesRead <= 0) {
		return (CLOSE);
	}
	while ((line = getBufferNextLine(buffer, bytesRead, offset, m_parseState == BODY)) != NULL) {

		if (m_parseState == FIRST_LINE) {
			ByteStreamIt firstSpace = std::find(line->begin(), line->end(), ' ');
			m_requestMethod = strip(std::string(line->begin(), firstSpace));
			ByteStreamIt questionMark = std::find(std_next(firstSpace), line->end(), '?');
			ByteStreamIt secondSpace = std::find(std_next(firstSpace), line->end(), ' ');
			if (questionMark != line->end()) {
				m_URL = decodeUrl(strip(std::string(std_next(firstSpace), questionMark)));
				m_queryString = strip(std::string(std_next(questionMark), secondSpace));
			} else {
				m_URL = decodeUrl(strip(std::string(std_next(firstSpace), secondSpace)));
			}
			m_parseState = HEADERS;
		} else if (m_parseState == HEADERS) {
			std::string lineStr = std::string(line->begin(), line->end());
			if (startsWith("Host:", lineStr)) {
				m_domain = strip(lineStr.substr(lineStr.find(":") + 2));
			} else if (startsWith("Content-Length:", lineStr)) {
				m_contentLength = lineStr.substr(lineStr.find(":") + 2);
			} else if (startsWith("Content-Type:", lineStr)) {
				m_requestContentType = strip(lineStr.substr(lineStr.find(":") + 2));
			} else if (lineStr == "\n" || lineStr == "\r\n") {
				m_parseState = BODY;
			}
		} else {
			m_requestBody.insert(m_requestBody.end(), line->begin(), line->end());
		}
		delete (line);
	}
	if (static_cast<int>(m_requestBody.size()) >= std::atoi(m_contentLength.c_str())) {
		return (REQUEST_READ);
	}
	return (REQUEST_NOT_READ);
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

	switch (readRequest()) {

		case CLOSE:
			return (CLOSE);
		case REQUEST_READ:
			m_requestStatus = REQUEST_READ;
			break;
		default:
			return (REQUEST_NOT_READ);
	}
	assignSettings(serverBlocks);
	if (unknownMethod()) {
		buildErrorPage(http::NOT_IMPLEMENTED_501);
		return (ERROR);
	}
	StrVector allowedMethods = m_settings.getAllowedMethods();
	if (std::find(allowedMethods.begin(), allowedMethods.end(),
		m_requestMethod) == allowedMethods.end()) {
			buildErrorPage(http::FORBIDDEN_403);
			return (ERROR);
		} else if (std::atoi(m_contentLength.c_str()) > m_maxBodySize) {
			buildErrorPage(http::CONTENT_TOO_LARGE_413);
			return (ERROR);
		}
	if (m_settings.getRedirection().first != "" && m_settings.getRedirection().second != "") {
		m_statusCode = m_settings.getRedirection().first;
		m_URL = m_settings.getRedirection().second;
	}
	m_filePath = m_settings.getRoot() + m_URL;

	if (m_requestMethod == "DELETE") {
		if (std::remove(m_filePath.c_str()) != 0) {
			buildErrorPage(NOT_FOUND_404);
			return (http::ERROR);
		}
		m_statusCode = OK_200;
		m_responseBody = basicHtml("Success Deleting File", "<h2>Success Deleting File</h2>");
		return (http::OK);
	}
	if (m_URL.find("/cgi-bin") != m_URL.npos) {
		try {
			performCgi();
		}
		catch (const CgiNotFound&) {
			buildErrorPage(NOT_FOUND_404);
			return (http::ERROR);
		}
			catch (const std::exception& e) {
				buildErrorPage(INTERNAL_ERROR_500);
				return (http::ERROR);
			}
			return (http::CGI);
		}
		if (isDirectory(m_filePath)) {
			m_filePath += m_settings.getIndexFile();
			if (!isFile(m_filePath) && m_settings.getDirectoryListing()) {
				performDirectoryListing();
				return (http::OK);
			}
		}
		std::ifstream requestedFile(m_filePath.c_str());
		if (requestedFile.fail() || isDirectory(m_filePath)) {
			buildErrorPage(NOT_FOUND_404);
			return (http::ERROR);
		}
		ifstreamToString(requestedFile, m_responseBody);
		requestedFile.close();
		m_statusCode = OK_200;
		return (http::OK);
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

			if (m_port != it->getPort()
				|| (normalizeHostname(it->getHostname()) != "0.0.0.0" 
				&& normalizeHostname(m_hostname) != normalizeHostname(it->getHostname()))) {
					continue;
			}
			if (i == 0) {
				defaultServerBlock = &(*it);
				i++;
			}
			std::stringstream	strStream;
			strStream << it->getPort();
			std::string	domain(it->getServerName() + ":" + strStream.str());
			if (m_domain == domain) {
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
	m_maxBodySize = selectedServerBlock->getMaxBodySize();
}


/**
 * @brief Builds an error page for the client HTTP request
 * 
 * @param errorCode The error code of the response
 * 
*/
void	HttpRequest::buildErrorPage(const std::string& errorCode) {

	std::ifstream errorFile((m_errorPages[errorCode]).c_str());

	if (errorFile.fail()) {
		std::cout << strerror(errno) << std::endl;
		m_responseBody = basicHtml("500", "<h2>500 Internal Server Error</h2>");
		m_statusCode = "500";
		return;
	}

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
	htmlBody += "\t<div class=\"container py-5 text-center\">\n";
	htmlBody += "\t<ul class=\"list-group\">\n";
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
		htmlBody +=	"\t\t<li class=\"list-group-item\">\n"
						"\t\t\t<a href=\"" + path + "\" >" + entryName + "</a>\n"
					"\t\t</li>\n";
	} while (entry);
	closedir(directory);
	htmlBody += "\t</ul>\n";
	htmlBody += "\t</div>\n";
	m_responseBody = basicHtml("Directory Listing", htmlBody);
	m_contentType = "text/html";
	return (true);
}


void	HttpRequest::performCgi() {

	int		outputPipe[2], inputPipe[2];
	pid_t	proccessID;

	if (pipe(outputPipe) == -1 || (m_requestMethod == "POST" && pipe(inputPipe) == -1)) {
		throw CgiError();
	}
	FileDescriptor(outputPipe[0]).setNonBlocking();

	if (m_requestMethod == "POST") {
		write(inputPipe[1], m_requestBody.data(), std::atoi(m_contentLength.c_str()));
		close(inputPipe[1]);
	}

	proccessID = fork();
	if (proccessID == -1) {
		throw CgiError();
	} else if (proccessID == 0) {
		childProccess(outputPipe, inputPipe);
	} else {
		if (m_requestMethod == "POST")
			close(inputPipe[0]);
		close(outputPipe[1]);
		int status = waitForProccess(proccessID);
		if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
			if (WEXITSTATUS(status) == ENOENT) {
				throw CgiNotFound();
			}
			throw CgiError();
		} else {
			readResponseFromCgi(outputPipe);
		}
	}
}


void	HttpRequest::readResponseFromCgi(int outputPipe[2]) {

	char	buffer[1024];
	ssize_t	bytesRead;
	
	while (true) {
		bytesRead = read(outputPipe[0], buffer, sizeof(buffer));
		if (bytesRead == -1) {
			throw CgiError();
		} else if (bytesRead == 0) {
			break;
		} else {
			m_response.append(buffer, bytesRead);
		}
	}
	close (outputPipe[0]);
}


void	HttpRequest::childProccess(int outputPipe[2], int inputPipe[2]) {

	if (dup2(outputPipe[1], STDOUT_FILENO) == -1 || (m_requestMethod == "POST"
			&& dup2(inputPipe[0], STDIN_FILENO) == -1)) {
				std::exit(1);
			}
	if (m_requestMethod == "POST")
		close(inputPipe[1]);
	close(outputPipe[0]);

	StrVector	argvVector;

	argvVector.push_back("/usr/bin/python3");
	argvVector.push_back(m_URL.substr(1));

	char** argv = vectorToCharPtrArr(argvVector);
	char** env = createEnvironment();

	chdir(m_settings.getRoot().c_str());
	execve(argv[0], argv, env);
	int status = errno;
	cleanCharPtrArr(argv);
	cleanCharPtrArr(env);
	std::exit(status);
}

int	HttpRequest::waitForProccess(pid_t& proccessID) {

	timeval		start, now;
	long long	elapsed;
	int			status = 0;

	gettimeofday(&start, NULL);

	while (true) {
		if (waitpid(proccessID, &status, WNOHANG) > 0) {
			break;
		}
		gettimeofday(&now, NULL);
		elapsed = (now.tv_sec - start.tv_sec) * 1000000LL + (now.tv_usec - start.tv_usec);
		if (elapsed >= 4000000) {
			kill(proccessID, SIGTERM);
		}
		usleep(200000);
	}
	return (status);
}


char**	HttpRequest::createEnvironment() {

	std::vector<std::string>	envVector;

	envVector.push_back("PATH_INFO=" + m_filePath);
	envVector.push_back("DOMAIN=" + m_domain);
	envVector.push_back("REQUEST_METHOD=" + m_requestMethod);
	envVector.push_back("QUERY_STRING=" + m_queryString);
	envVector.push_back("CONTENT_LENGTH=" + m_contentLength);
	envVector.push_back("CONTENT_TYPE=" + m_requestContentType);
	envVector.push_back("UPLOAD_DIR=" + m_settings.getUploadedFilesPath());

	return (vectorToCharPtrArr(envVector));
}


/**
 * @brief Sends the HTTP response over a socket to a client
 *
*/
void	HttpRequest::sendResponse() {

	if (m_requestStatus != http::CGI) {
		m_response =	m_version + " " + expandStatusCode() + "\r\n"
						"Content-Type: " + expandContentType() + "\r\n"
						"Content-Length: " + expandContentLength() + "\r\n"
						"\r\n" + m_responseBody;
	}
	write(m_targetSocketFileDescriptor, m_response.c_str(), m_response.length());
	m_requestStatus = CLOSE;
}

bool HttpRequest::unknownMethod() {
	return (m_requestMethod != "GET" && m_requestMethod != "POST"
				&& m_requestMethod != "DELETE");
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


const char* HttpRequest::CgiError::what() const throw() {
	return ("Cgi Error");
}

const char* HttpRequest::CgiNotFound::what() const throw() {
	return ("Cgi Not Found");
}
