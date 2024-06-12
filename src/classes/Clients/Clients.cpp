#include "Clients.hpp"
#include "../EventPoll/EventPoll.hpp"

using namespace http;

void	Clients::addToSocketMap(const FileDescriptor& fd, HttpRequest* request) {
	m_socketToRequestMap.insert(std::make_pair(fd, request));
}

void	Clients::addToPipeMap(const FileDescriptor& fd, HttpRequest* request) {
	m_pipeReadToRequestMap.insert(std::make_pair(fd, request));
}

HttpRequest*	Clients::delFromSocketMap(const FileDescriptor& fd) {

	http::Iterator it = m_socketToRequestMap.find(fd);
	if (it == m_socketToRequestMap.end()) {
		throw MapEntryNotFound();
	}
	HttpRequest* request = it->second;
	m_socketToRequestMap.erase(it);
	return (request);
}

HttpRequest*	Clients::delFromPipeMap(const FileDescriptor& fd) {

	http::Iterator it = m_pipeReadToRequestMap.find(fd);
	if (it == m_pipeReadToRequestMap.end()) {
		throw MapEntryNotFound();
	}
	HttpRequest* request = it->second;
	m_pipeReadToRequestMap.erase(it);
	return (request);
}


void	Clients::removeClosedConnections(EventPoll& eventManager) {

	timeval		now;
	long long	elapsed;

	gettimeofday(&now, NULL);

	http::FdReqMap::iterator it = m_socketToRequestMap.begin();
	http::FdReqMap::iterator next;

	while (it != m_socketToRequestMap.end()) {
		elapsed = (now.tv_sec - it->second->getStartTimeRequest().tv_sec) * 1000000LL +
			(now.tv_usec - it->second->getStartTimeRequest().tv_usec);
		next = std_next(it);
		if (it->second->getRequestStatus() == http::CLOSE || elapsed > 5000000) {
			eventManager.remove(it->first);
			delete it->second;
			m_socketToRequestMap.erase(it);
		} else {
			if (it->second->monitorCgiRunTime() == http::CGI_ERROR) {
				it->second->buildErrorPage(http::INTERNAL_ERROR_500);
				it->second->setRequestStatus(http::ERROR);
				eventManager.mod(it->first, WRITE_OPERATIONS);
			}
		}
		it = next;
	}
}

FileDescriptor Clients::getSocketByPipe(FileDescriptor pipeRead) {

	http::HttpRequest* request = m_pipeReadToRequestMap[pipeRead];

	return (getSocketByRequest(request));
}

FileDescriptor Clients::getSocketByRequest(http::HttpRequest* request) {

	http::Iterator it = m_socketToRequestMap.begin();
	while (it != m_socketToRequestMap.end()) {
		if (it->second == request) {
			return (it->first);
		}
		it++;
	}
	throw MapEntryNotFound();
}


HttpRequest*& Clients::operator[](const FileDescriptor& key) {

	http::Iterator it = m_socketToRequestMap.find(key);
	if (it != m_socketToRequestMap.end()) {
		return (it->second);
	}

	http::Iterator it2 = m_pipeReadToRequestMap.find(key);
	if (it2 != m_pipeReadToRequestMap.end()) {
		return (it2->second);
	}
	throw MapEntryNotFound();
}


const char* Clients::MapEntryNotFound::what() const throw() {
	return ("Map entry not found");
}
