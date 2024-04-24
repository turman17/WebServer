#ifndef CLIENTS_HPP
# define CLIENTS_HPP

# include "stdlibraries.hpp"
# include "../HttpRequest/HttpRequest.hpp"

class EventPoll;

class Clients {
public:
	void			addToSocketMap(const FileDescriptor& fd, http::HttpRequest* request);
	void			addToFilesMap(const FileDescriptor& fd, http::HttpRequest* request);
	http::HttpRequest*	delFromSocketMap(const FileDescriptor& fd);
	http::HttpRequest*	delFromFilesMap(const FileDescriptor& fd);
	void			removeClosedConnections(EventPoll& eventManager);	
	bool			isSocket(const FileDescriptor& fd);

	http::HttpRequest* & operator[](const FileDescriptor& key);
	
private:
	std::map<const FileDescriptor, http::HttpRequest*>	m_socketToRequestMap;
	std::map<const FileDescriptor, http::HttpRequest*>	m_fileRequestedToRequestMap;

	class MapEntryNotFound : std::exception {
	public:
		const char* what() const throw();
	};

};

#endif
