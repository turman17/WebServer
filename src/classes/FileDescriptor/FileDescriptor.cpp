#include "FileDescriptor.hpp"
#include "../FileDescriptor/FileDescriptor.hpp"


namespace unistd {
	/**
     *  @brief Closes a file descriptor.
     * 
     * @param fd The file descriptor to close.
     * @return 0 on success, -1 on failure.
     */
	int close (int fd) {
		return (::close(fd));
	}
};

FileDescriptor::FileDescriptor() : m_fd(-1) , m_isOpen(false) {}


FileDescriptor::FileDescriptor(const int& fd) : m_fd(fd), m_isOpen(false) {

	if (m_fd == -1) {
		return;
	}
	if (fcntl(m_fd, F_GETFL) != -1) {
		m_isOpen = true;
	}
}


FileDescriptor::FileDescriptor(const FileDescriptor& other) {

	if (this != &other) {
		*this = other;
	}
}


FileDescriptor& FileDescriptor::operator=(const FileDescriptor& other) {

	if (this != &other) {
		this->m_fd = other.m_fd;
		this->m_isOpen = other.m_isOpen;
	}
	return (*this);
}


FileDescriptor::~FileDescriptor() {
	this->close();
}


const int&	FileDescriptor::get() const {
	return (m_fd);
}


void	FileDescriptor::set(const int& fd) {

	m_fd = fd;
	if (m_fd == -1) {
		m_isOpen = false;
		return;
	}

	if (fcntl(m_fd, F_GETFL) != -1) {
		m_isOpen = true;
	}
	else {
		m_isOpen = false;
	}
}


void	FileDescriptor::close() const {
	if (m_isOpen) {
		unistd::close(m_fd);
	}
	m_isOpen = false;
}

void	FileDescriptor::setNonBlocking() const {

	int flags = fcntl(m_fd, F_GETFL, 0);

	if (!(flags & O_NONBLOCK)) {
		flags |= O_NONBLOCK;
		fcntl(m_fd, F_SETFL, flags);
	}
}


bool FileDescriptor::operator<(int fd) const { return (m_fd < fd); }
bool FileDescriptor::operator>(int fd) const { return (m_fd > fd); }
bool FileDescriptor::operator<=(int fd) const { return (m_fd <= fd); }
bool FileDescriptor::operator>=(int fd) const { return (m_fd >= fd); }
bool FileDescriptor::operator==(int fd) const { return (m_fd == fd); }
bool FileDescriptor::operator<(const FileDescriptor& other) const { return (m_fd < other.m_fd); }
bool FileDescriptor::operator>(const FileDescriptor& other) const { return (m_fd > other.m_fd); }
bool FileDescriptor::operator<=(const FileDescriptor& other) const { return (m_fd <= other.m_fd); }
bool FileDescriptor::operator>=(const FileDescriptor& other) const { return (m_fd >= other.m_fd); }
bool FileDescriptor::operator==(const FileDescriptor& other) const { return (m_fd == other.m_fd); }
