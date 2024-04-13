#ifndef CLIENTS_HPP
# define CLIENTS_HPP

# include "webserv.hpp"

class HttpRequest;

typedef std::pair<const FileDescriptor, HttpRequest*> FdReqPair;
typedef std::map<const FileDescriptor, HttpRequest*> FdReqMap;
typedef std::map<const FileDescriptor, HttpRequest*>::iterator Iterator;

class Clients {
public:
	void		addToSocketMap(const FileDescriptor& fd, HttpRequest* request);
	void		addToFilesMap(const FileDescriptor& fd, HttpRequest* request);
	FdReqPair	delFromSocketMap(const FileDescriptor& fd);
	FdReqPair	delFromFilesMap(const FileDescriptor& fd);
	bool		isSocket(const FileDescriptor& fd);

	HttpRequest*& operator[](const FileDescriptor& key);
	
private:
	std::map<const FileDescriptor, HttpRequest*>	m_socketToRequestMap;
	std::map<const FileDescriptor, HttpRequest*>	m_fileRequestedToRequestMap;

	class MapEntryNotFound : std::exception {
	public:
		const char* what() const throw();
	};

};

#endif
