#ifndef EVENTPOLL_HPP
# define EVENTPOLL_HPP
 
# include "webserv.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"

class ServerSocket;

class EventPoll {

public:
	EventPoll(ServerSocket* controlSocket);
	~EventPoll();
	void	add(const int& fileDescriptor, uint32_t eventsToNotify);
	void	mod(const int& fileDescriptor, uint32_t eventsToNotify);
	void	remove(const int& fileDescriptor);
	//TODO Somehow integrate epoll_wait within the class
private:
	const FileDescriptor	m_fileDescriptor;
	const ServerSocket		*m_controlSocket;


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
};

#endif
