#ifndef EVENTPOLL_HPP
# define EVENTPOLL_HPP
 
# include "webserv.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"

using namespace epoll;

class ServerSocket;

class EventPoll {

public:
					EventPoll(ServerSocket* controlSocket);
					~EventPoll();
	void			add(const FileDescriptor& fileDescriptor, uint32_t eventsToNotify) const;
	void			mod(const FileDescriptor& fileDescriptor, uint32_t eventsToNotify) const;
	void			remove(const FileDescriptor& fileDescriptor) const;
	void			waitForEvents() const;
	FileDescriptor	getNextEvent(int &eventType) const;
private:
	const FileDescriptor										m_fileDescriptor;
	const ServerSocket*											m_controlSocket;

	class Event : public FileDescriptor {
	public:
		Event();
		Event(const FileDescriptor& fd, const uint32_t& events);
		uint32_t getEvents() const;
	private:
		uint32_t	m_events;
	};

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
