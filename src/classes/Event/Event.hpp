#ifndef EVENT_HPP
# define EVENT_HPP

# include "stdlibraries.hpp"

# include "../ServerSocket/ServerSocket.hpp"

class ServerSocket;

class Event : public FileDescriptor {
public:
					Event();
					Event(const FileDescriptor& fd,
						const uint32_t& events, ServerSocket* serverSocket);
	uint32_t		getEvents() const;
	void			setEvents(const uint32_t& events);
	void			setServerSocket(ServerSocket* serverSocket);
	ServerSocket*	getServerSocket() const;
	int			fd() const;
private:
	uint32_t		m_events;
	ServerSocket*	m_serverSocket;	
};

bool operator==(const ServerSocket* serverSocket, const Event event);

#endif
