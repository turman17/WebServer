#include "EventPoll.hpp"
#include "../ServerSocket/ServerSocket.hpp"
#include "../FileDescriptor/FileDescriptor.hpp"


EventPoll::EventPoll() : m_fileDescriptor(-1), m_controlSocket(NULL) {}


EventPoll::~EventPoll() {

	m_fileDescriptor.close();
}


/**
 * @brief Creates a new `EventPoll`
 * 
 * @param controlSocket The `ServerSocket` to where the clients will connect
 * 
 * @throw  In case of an error throws and exception of type `EventPollException` (std::exception)
 * 
*/
EventPoll::EventPoll(ServerSocket* controlSocket)
	: m_fileDescriptor(epoll_create(MAX_EVENTS)), m_controlSocket(controlSocket) {

	if (m_fileDescriptor < 0) {
		throw EventPollException("epoll_create()" + std::string (std::strerror(errno)));
	}

	add(m_controlSocket->getFileDescriptor(), CAN_READ);
}


/**
 * @brief Add a new file descriptor to the `EventPoll`
 * 
 * @param fileDescriptor the file descriptor to be added
 * @param eventsToNotify the type of events to be notified for
 *
 * @throw  In case of an error throws and exception of type `EventPollException` (std::exception)
 * 
*/ 
void	EventPoll::add(const FileDescriptor& fileDescriptor, uint32_t eventsToNotify) const {

	epoll_event eventSettings;

	std::memset(&eventSettings, 0, sizeof(eventSettings));
	eventSettings.data.fd = fileDescriptor;
	eventSettings.events = eventsToNotify;

	if (epoll_ctl(m_fileDescriptor,
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
 * @throw  In case of an error throws and exception of type `EventPollException` (std::exception)
 * 
*/
void	EventPoll::mod(const FileDescriptor& fileDescriptor, uint32_t eventsToNotify) const {

	epoll_event eventSettings;

	std::memset(&eventSettings, 0, sizeof(eventSettings));
	eventSettings.data.fd = fileDescriptor;
	eventSettings.events = eventsToNotify;

	if (epoll_ctl(m_fileDescriptor,
		EPOLL_CTL_MOD, fileDescriptor, &eventSettings) == -1) {
			throw EventPollException("epoll_ctl(MOD)" + std::string (std::strerror(errno)));
		}
}

/**
 * @brief Remove a file descriptor inside the `EventPoll`
 * 
 * @param fileDescriptor the file descriptor to be removed
 * 
 * @throw  In case of an error throws and exception of type `EventPollException` (std::exception)
 * 
*/
void	EventPoll::remove(const FileDescriptor& fileDescriptor) const {

	if (epoll_ctl(m_fileDescriptor,
		EPOLL_CTL_DEL, fileDescriptor, NULL) == -1) {
			throw EventPollException("epoll_ctl(DEL) " + std::string (std::strerror(errno)));
		}
	fileDescriptor.close();
}


/**
 * @brief Starts waiting for new event notification
 * 
 * @throw  In case of an error throws and exception of type `EventPollException` (std::exception)
 * 
*/
void	EventPoll::waitForEvents() const {

	epoll_event newEvents[MAX_EVENTS / 4];

	int newEventsNum = epoll_wait(m_fileDescriptor, newEvents, MAX_EVENTS / 4, -1);
	if (newEventsNum == -1) {
		throw EventPollException("epoll_wait()" + std::string (std::strerror(errno)));
	}

	for (int i = 0; i < newEventsNum; i++) {
		m_newEvents.push_back(FileDescriptor(newEvents[i].data.fd));
	}
}


/**
 * @return Returns the next file descriptor where there is a notification
 * 
 * @throw  Whenever there's no more events, throws and exception of type `NoMoreNewEvents`
 * 
*/
FileDescriptor	EventPoll::getNextEvent(int &eventType) const {
	
	if (m_newEvents.isEmpty()) {
		throw NoMoreNewEvents();
	} 
	FileDescriptor nextEvent = m_newEvents.front();
	m_newEvents.pop_front();

	if (nextEvent == m_controlSocket->getFileDescriptor()) {
		eventType = NEW_CONNECTION;
	}
	// TODO Continue whenever clients class is ready

	return (nextEvent);
}


//* Exceptions


EventPoll::EventPollException::EventPollException
	(const std::string& errorMessage) : m_errorMessage(
		"EventPollException: " + errorMessage) {}


const char* EventPoll::EventPollException::what() const throw() {
	return (m_errorMessage.c_str());
}


EventPoll::EventPollException::~EventPollException() throw() {}


const char* EventPoll::NoMoreNewEvents::what() const throw() {
	return ("No more new events");
}
