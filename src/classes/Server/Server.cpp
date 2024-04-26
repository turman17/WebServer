# include "Server.hpp"

Server::Server() {}

Server::~Server() {
	//! DELETE SERVER SOCKETS
}

void	Server::run() {
	Event				newEvent;
	http::RequestStatus	status;
	http::HttpRequest	*activeRequest;

	setupListeningSockets();
	setupEpoll();
	while (true) {
		m_eventsManager.waitForEvents();
		while (NEW_EVENTS) {
			try {
				newEvent = m_eventsManager.getNextEvent();
				std::cout << "New event on fd: " << newEvent.fd() << std::endl;
				if (newEvent.isNewConnection(m_listeningSockets)) {
					acceptNewConnection(newEvent);
				} else if (newEvent.isReadable()) {
					activeRequest = m_clientsMap[newEvent.fd()];
					status = activeRequest->performReadOperations(m_serverBlocks);
					activeRequest->setRequestStatus(status);
					m_eventsManager.mod(newEvent.fd(), WRITE_OPERATIONS);
				} else if (newEvent.isWritable()) {
					sleep(1);
					std::cout << "Sending response if possible" << std::endl;
					activeRequest = m_clientsMap[newEvent.fd()];
					status = activeRequest->sendResponse();
					if (status == http::RESPONSE_SENT) {
						activeRequest->setRequestStatus(http::CLOSED);
					}
				}
			}
			catch (const std::exception& exception) {
				std::cerr << exception.what() << std::endl;
				m_clientsMap.removeClosedConnections(m_eventsManager);
				break;
			}
		}
	}
}


void	Server::acceptNewConnection(const Event& event) {

	FileDescriptor	newConnectionFd;

	newConnectionFd = accept(event.fd(), NULL, NULL);
	newConnectionFd.setNonBlocking();

	http::HttpRequest* newRequest = new http::HttpRequest(newConnectionFd);
	newRequest->setPort(event.getPort());
	newRequest->setHostname(event.getHostname());
	m_clientsMap.addToSocketMap(newConnectionFd, newRequest);
	m_eventsManager.add(newConnectionFd, READ_OPERATIONS);
}


void	Server::loadConfig(const char* filename) {

	std::ifstream						configFile(filename);
	std::map<std::string, std::string>	ConfigMap;
	std::string							line = "";

	if (configFile.fail()) {
		throw BadOpenFile();
	}

	while (!line.empty() || std::getline(configFile, line)) {

		if (line == "server:") {
			ServerBlock newServerBlock;
			while (std::getline(configFile, line) && !std::isalpha(line[0])) {
				if (isOnlySpaces(line) || line[0] == '#') {
					continue;
				}
				if (startsWith(std::string("location"), line)) {
					LocationBlock 		newLocationBlock;
					newLocationBlock.setRoot(newServerBlock.getRoot());
					std::stringstream	tmpStream(line);
					std::string			tmpStr;

					tmpStream >> tmpStr;
					tmpStream >> tmpStr;
					newLocationBlock.setRoutePath(tmpStr);
					while (std::getline(configFile, line) && line[0] == '\t' && line[1] == '\t') {
						if (isOnlySpaces(line) || line[0] == '#') {
							continue;
						}
						assignLocationBlockSetting(line, newLocationBlock);
					}
					newServerBlock.addLocationBlock(newLocationBlock);
				}
				else {
					assignServerBlockSetting(line, newServerBlock);
				}
			}
			m_serverBlocks.push_back(newServerBlock);
		}
	}
}

void	Server::assignServerBlockSetting(const std::string& line, ServerBlock& serverBlock) {

	std::stringstream	lineStream(line);
	std::string			directive = "";
	std::string			value = "";
	std::string			tmp = "";

	lineStream >> directive;
	std::getline(lineStream, value, ';');
	std::stringstream	valueStream(value);

	if (directive == "listen") {
		if (value.find(':') || std::isalpha(value[0])) {
			std::getline(valueStream >> std::ws, tmp, ':');
			serverBlock.setHostname(tmp);
		}
		if (value.find(':') || std::isdigit(value[0])) {
			valueStream >> tmp;
			serverBlock.setPort(std::atoi(tmp.c_str()));
		}
		if (valueStream.fail())
			throw BadConfig();
	}
	else if (directive == "server_name") {
		valueStream >> tmp;
		if (valueStream.fail())
			throw BadConfig();
		serverBlock.setServerName(tmp);
	}
	else if (directive == "root") {
		valueStream >> tmp;
		if (valueStream.fail())
			throw BadConfig();
		serverBlock.setRoot(tmp);
	}
	else if (directive == "error_page") {
		if (countWords(value) != 2) {
			throw BadConfig();
		}
		std::string errorCode;
		valueStream >> errorCode;
		valueStream >> tmp;
		if (valueStream.fail())
			throw BadConfig();
		if (isFile(tmp))
			serverBlock.addErrorPage(errorCode, tmp);
	}
	else if (directive == "client_max_body_size") {
		valueStream >> tmp;
		if (valueStream.fail())
			throw BadConfig();
		serverBlock.setMaxBodySize(std::atoi(tmp.c_str()));
	}
	else {
		std::cout << directive << std::endl;
		throw UnknownDirective();
	}
}

void	Server::assignLocationBlockSetting(const std::string& line, LocationBlock& locationBlock) {

	std::stringstream	lineStream(line);
	std::string			directive = "";
	std::string			value = "";
	std::string			tmp = "";

	lineStream >> directive;
	std::getline(lineStream, value, ';');
	std::stringstream	valueStream(value);

	if (directive == "root") {
		valueStream >> tmp;
		if (valueStream.fail())
			throw BadConfig();
		locationBlock.setRoot(tmp);
	}
	else if (directive == "index") {
		valueStream >> tmp;
		if (valueStream.fail())
			throw BadConfig();
		locationBlock.setIndexFile(tmp);
	}
	else if (directive == "return") {
		std::string statusCode;
		valueStream >> statusCode;
		valueStream >> tmp;
		if (valueStream.fail())
			throw BadConfig();
		locationBlock.setRedirection(std::make_pair(statusCode, tmp));
	}
	else if (directive == "allow") {
		StrVector	tmpVector;
		while (valueStream >> tmp) {
			tmpVector.push_back(tmp);
		}
		locationBlock.setAllowedMethods(tmpVector);
	}
	else if (directive == "autoindex") {
		valueStream >> tmp;
		if (valueStream.fail() || (tmp != "on" && tmp != "off"))
			throw BadConfig();
		locationBlock.setDirectoryListing(tmp == "on" ? true: false);
	}
	else if (directive == "client_body_temp_path") {
		valueStream >> tmp;
		if (valueStream.fail())
			throw BadConfig();
		locationBlock.setUploadedFilesPath(tmp);
	}
	else {
		throw UnknownDirective();
	}
}

void	Server::setupListeningSockets() {

	for (std::vector<ServerBlock>::iterator current = m_serverBlocks.begin();
		current != m_serverBlocks.end(); std::advance(current, 1)) {
			try {
				ServerSocket	newSocket((*current).getHostname(), (*current).getPort());
				if (newSocket.isOpen()) {
					std::cout << "Listening on " << (*current).getHostname() << ":" << (*current).getPort() << std::endl;
					m_listeningSockets.push_back(newSocket);
				}
			}
			catch (const std::exception& exception) {
				std::cerr << "Error creating socket: " << exception.what() << std::endl;
			}
		}
}


void Server::setupEpoll() {

	for (std::vector<ServerSocket>::iterator current = m_listeningSockets.begin();
		current != m_listeningSockets.end(); std::advance(current, 1)) {
			m_eventsManager.add((*current).getFileDescriptor(), READ_OPERATIONS);
		}
}


const char* Server::BadOpenFile::what() const throw() {
	
	return ("Error opening file");
}

const char* Server::UnknownDirective::what() const throw() {
	
	return ("Unknown directive");
}

const char* Server::BadConfig::what() const throw() {

	return ("Error reading config");
}


void	Server::printSettings() {
	
	std::cout << "Printing settings" << std::endl;
	for (std::vector<ServerBlock>::iterator it = m_serverBlocks.begin();
			it != m_serverBlocks.end(); std::advance(it, 1)) {
				std::cout << "SERVER BLOCK:" << std::endl;
				std::cout << "port: " << (*it).getPort() << std::endl;
				std::cout << "hostname: " << (*it).getHostname() << std::endl;
				std::cout << "server name: " << (*it).getServerName() << std::endl;
				std::cout << "root: " << (*it).getRoot() << std::endl;
				std::cout << "max body size: "<< (*it).getMaxBodySize() << std::endl;
				if (!(*it).getErrorPages().empty())
					std::cout << "error pages: " << std::endl;
				for (std::map<std::string, std::string>::const_iterator it2 = (*it).getErrorPages().begin();
						it2 != (*it).getErrorPages().end(); std::advance(it2, 1)) {
							std::cout << "  " << it2->first << " - ";
							std::cout << it2->second << std::endl;
						}
				std::vector<LocationBlock> tmp = (*it).getLocationBlocks();
				for (std::vector<LocationBlock>::iterator it = tmp.begin(); it != tmp.end(); std::advance(it, 1)) {
					std::cout << "\nLOCATION BLOCK:" << std::endl;
					std::cout << '\t' << "route to: " << (*it).getRoutePath() << std::endl;
					std::cout << '\t' << "root: " << (*it).getRoot() << std::endl;
					std::cout << '\t' << "index: " << (*it).getIndexFile() << std::endl;
					std::cout << '\t' << (*it).getUploadedFilesPath() << std::endl;
					std::pair<std::string, std::string> tmpPair = (*it).getRedirection();
					std::cout << '\t' << tmpPair.first << std::endl;
					std::cout << '\t' << tmpPair.second << std::endl;
					std::vector<std::string> tmpVector = (*it).getAllowedMethods();
					std::cout << '\t' << "Allowed Methods" << std::endl; 
					for (std::vector<std::string>::iterator it = tmpVector.begin(); it != tmpVector.end(); std::advance(it, 1)) {
						std::cout << '\t' << *it << std::endl;
					}
					std::cout << '\t' << (*it).getDirectoryListing() << std::endl;;
			}
	}
}
