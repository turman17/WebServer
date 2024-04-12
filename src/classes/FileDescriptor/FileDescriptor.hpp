#ifndef FILEDESCRIPTOR_HPP
# define FILEDESCRIPTOR_HPP

# include <unistd.h>
# include <fcntl.h>

class FileDescriptor {

public:
	FileDescriptor();
	FileDescriptor(const int& fd);
	FileDescriptor(const FileDescriptor& other);
	FileDescriptor& operator=(const FileDescriptor& other);
	operator int() const;
	bool operator<(int fd) const;
	bool operator>(int fd) const;
	bool operator<=(int fd) const;
	bool operator>=(int fd) const;
	bool operator==(int fd) const;
	bool operator<(const FileDescriptor& other) const;
	bool operator>(const FileDescriptor& other) const;
	bool operator<=(const FileDescriptor& other) const;
	bool operator>=(const FileDescriptor& other) const;
	bool operator==(const FileDescriptor& other) const;

	~FileDescriptor();
	const int& 	get() const;
	void		set(const int& fd);
	void		close() const;
	void		setNonBlocking() const;

private:
	int 		m_fd;
	mutable int	m_isOpen;
};

#endif
