#include "Clients.hpp"
#include "../EventPoll/EventPoll.hpp"

using namespace http;

void	Clients::addToSocketMap(const FileDescriptor& fd, HttpRequest* request) {
	m_socketToRequestMap.insert(std::make_pair(fd, request));
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
		}
		it = next;
	}
}


HttpRequest*& Clients::operator[](const FileDescriptor& key) {

	http::Iterator it = m_socketToRequestMap.find(key);
	if (it != m_socketToRequestMap.end()) {
		return (it->second);
	}
	throw MapEntryNotFound();
}


const char* Clients::MapEntryNotFound::what() const throw() {
	return ("Map entry not found");
}
