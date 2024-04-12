#ifndef UTILS_HPP
# define UTILS_HPP

# include <exception>

template<typename Iterator>
Iterator std_next(const Iterator& it, int x = 1) {

	Iterator returnIterator = it;
	std::advance(returnIterator, x);
	return (returnIterator);
}

class BadFileDescriptor : public std::exception {
public:
	const char* what() const throw() {
		return ("Bad file descriptor");
	}
};

class BadRead : public std::exception {
public:
	const char* what() const throw() {
		return ("Bad read");
	}
};

#endif
