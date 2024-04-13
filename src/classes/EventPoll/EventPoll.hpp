#ifndef EVENTPOLL_HPP
# define EVENTPOLL_HPP
 
# include "stdlibraries.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"
# include "../ServerSocket/ServerSocket.hpp"
# include "../Event/Event.hpp"

namespace epoll {
	const int MAX_EVENTS = 256;
	const int CAN_READ = EPOLLIN;
	const int CAN_WRITE = EPOLLOUT;

	const bool NEW_EVENTS = true;

	enum EventType {
		NEW_CONNECTION = 0,
		READ_OPERATIONS= EPOLLIN,
		WRITE_OPERATIONS = EPOLLOUT
	};

	typedef std::vector<ServerSocket*>::const_iterator Iterator;
};

using namespace epoll;


class EventPoll {

public:
			EventPoll(std::vector<ServerSocket*>& controlSockets);
			~EventPoll();
	void	add(const FileDescriptor& fileDescriptor, uint32_t eventsToNotify) const;
	void	mod(const FileDescriptor& fileDescriptor, uint32_t eventsToNotify) const;
	void	remove(const FileDescriptor& fileDescriptor) const;
	void	waitForEvents() const;
	Event	getNextEvent() const;

private:
	const FileDescriptor				m_fileDescriptor;
	std::vector<ServerSocket*>			m_controlSockets;
	mutable CircularBuffer<Event, (MAX_EVENTS / 4)>			m_newEvents;


	//* Private Methods
	EventPoll();


	//* Exceptions
	class	EventPollException : public std::exception {
		public:
			EventPollException(const std::string& errorMessage);
			~EventPollException() throw();
			const char* what() const throw();
		private:
			const std::string	m_errorMessage;
	};

	class NoMoreNewEvents {
		public:
			const char* what() const throw();
	};
};

#endif
