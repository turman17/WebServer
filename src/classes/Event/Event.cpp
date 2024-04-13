#include "Event.hpp"

Event::Event() : FileDescriptor(), m_events(0), m_serverSocket(NULL) {}


Event::Event(const FileDescriptor& fd, const uint32_t& events, ServerSocket* serverBlockSocket) :
	FileDescriptor(fd), m_events(events), m_serverSocket(serverBlockSocket) {}


uint32_t Event::getEvents() const {
	return (m_events);
}


void		Event::setEvents(const uint32_t& events) {
	m_events = events;
}

int			Event::fd() const {
	return (this->get());
}


void	Event::setServerSocket(ServerSocket* serverSocket) {
	m_serverSocket = serverSocket;
}


ServerSocket*	Event::getServerSocket() const {
	return (m_serverSocket);
}


bool operator==(const ServerSocket* serverSocket, const Event event) {
	if (serverSocket) {
		return (serverSocket->getFileDescriptor() == event.get());
	}
	return (false);
}
