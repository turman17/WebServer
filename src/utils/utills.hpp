#ifndef UTILS_HPP
# define UTILS_HPP

# include <exception>

bool			startsWith(const std::string& toExpect, const std::string& text);
unsigned int	countWords(const std::string& str);
bool			isOnlySpaces(const std::string& str);
std::string		normalizeHostname(const std::string& hostname);

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

class UnexpectedDifference : public std::exception {
public:
	const char* what() const throw() {
		return ("Unexpected difference");
	}
};

#endif
