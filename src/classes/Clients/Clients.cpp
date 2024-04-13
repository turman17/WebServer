#include "Clients.hpp"


void	Clients::addToSocketMap(const FileDescriptor& fd, HttpRequest* request) {
	m_socketToRequestMap.insert(std::make_pair(fd, request));
}


void	Clients::addToFilesMap(const FileDescriptor& fd, HttpRequest* request) {
	m_fileRequestedToRequestMap.insert(std::make_pair(fd, request));
}


FdReqPair	Clients::delFromSocketMap(const FileDescriptor& fd) {

	Iterator it = m_socketToRequestMap.find(fd);
	if (it == m_socketToRequestMap.end()) {
		throw MapEntryNotFound();
	}
	FdReqPair pair = *it;
	m_socketToRequestMap.erase(it);
	return (pair);
}


FdReqPair	Clients::delFromFilesMap(const FileDescriptor& fd) {
	Iterator it = m_fileRequestedToRequestMap.find(fd);
	if (it == m_fileRequestedToRequestMap.end()) {
		throw MapEntryNotFound();
	}
	FdReqPair pair = *it;
	m_fileRequestedToRequestMap.erase(it);
	return (pair);
}


bool		Clients::isSocket(const FileDescriptor& fd) {
	Iterator it = m_socketToRequestMap.find(fd);
	if (it != m_socketToRequestMap.end()) {
		return (true);
	}
	return (false);
}


HttpRequest*& Clients::operator[](const FileDescriptor& key) {

	Iterator it = m_socketToRequestMap.find(key);
	if (it != m_socketToRequestMap.end()) {
		return (it->second);
	}
	it = m_fileRequestedToRequestMap.find(key);
	if (it != m_fileRequestedToRequestMap.end()) {
		return (it->second);
	}
	throw MapEntryNotFound();
}


const char* Clients::MapEntryNotFound::what() const throw() {
	return ("Map entry not found");
}
