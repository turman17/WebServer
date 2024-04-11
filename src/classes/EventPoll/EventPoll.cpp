#include "EventPoll.hpp"
#include "../ServerSocket/ServerSocket.hpp"


EventPoll::EventPoll() : m_fileDescriptor(-1), m_controlSocket(NULL) {}


EventPoll::~EventPoll() {

	m_fileDescriptor.close();
}


/**
 * @brief Creates a new `EventPoll`
 * 
 * @param controlSocket The `ServerSocket` to where the clients will connect
 * 
 * @throw  In case of an error throws and exception o type `EventPollException` (std::exception)
 * 
*/
EventPoll::EventPoll(ServerSocket* controlSocket)
	: m_fileDescriptor(epoll_create(MAX_EVENTS)), m_controlSocket(controlSocket) {

	if (m_fileDescriptor < 0) {
		throw EventPollException("epoll_create()" + std::string (std::strerror(errno)));
	}

	add(controlSocket->getFileDescriptor(), CAN_READ);
}


/**
 * @brief Add a new file descriptor to the `EventPoll`
 * 
 * @param fileDescriptor the file descriptor to be added
 * @param eventsToNotify the type of events to be notified for
 * 
 * @throw  In case of an error throws and exception o type `EventPollException` (std::exception)
 * 
*/
void	EventPoll::add(const int& fileDescriptor, uint32_t eventsToNotify) {

	epoll_event eventSettings;

	std::memset(&eventSettings, 0, sizeof(eventSettings));
	eventSettings.data.fd = fileDescriptor;
	eventSettings.events = eventsToNotify;

	if (epoll_ctl(m_fileDescriptor.get(),
		EPOLL_CTL_ADD, fileDescriptor, &eventSettings) == -1) {
			throw EventPollException("epoll_ctl(ADD)" + std::string (std::strerror(errno)));
		}
}


/**
 * @brief Modify the events for a file descriptor inside the `EventPoll`
 * 
 * @param fileDescriptor the file descriptor to be modified
 * @param eventsToNotify the new type of events to be notified for
 * 
 * @throw  In case of an error throws and exception o type `EventPollException` (std::exception)
 * 
*/
void	EventPoll::mod(const int& fileDescriptor, uint32_t eventsToNotify) {

	epoll_event eventSettings;

	std::memset(&eventSettings, 0, sizeof(eventSettings));
	eventSettings.data.fd = fileDescriptor;
	eventSettings.events = eventsToNotify;

	if (epoll_ctl(m_fileDescriptor.get(),
		EPOLL_CTL_MOD, fileDescriptor, &eventSettings) == -1) {
			throw EventPollException("epoll_ctl(MOD)" + std::string (std::strerror(errno)));
		}
}

/**
 * @brief Remove a file descriptor inside the `EventPoll`
 * 
 * @param fileDescriptor the file descriptor to be removed
 * 
 * @throw  In case of an error throws and exception o type `EventPollException` (std::exception)
 * 
*/
void	EventPoll::remove(const int& fileDescriptor) {

	if (epoll_ctl(m_fileDescriptor.get(),
		EPOLL_CTL_DEL, fileDescriptor, NULL) == -1) {
			throw EventPollException("epoll_ctl(DEL) " + std::string (std::strerror(errno)));
		}
}


//* Exceptions


EventPoll::EventPollException::EventPollException
	(const std::string& errorMessage) : m_errorMessage(
		"EventPollException: " + errorMessage) {}


const char* EventPoll::EventPollException::what() const throw() {
	return (m_errorMessage.c_str());
}


EventPoll::EventPollException::~EventPollException() throw() {}
