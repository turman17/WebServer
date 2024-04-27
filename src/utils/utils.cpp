#include "stdlibraries.hpp"

void	fatalError(int _errno, void (*atExit)(void *), void* trash) {
	std::cerr << "Error: " << strerror(_errno) << " (fatal)" << std::endl;
	if (atExit && trash)
		atExit(trash);
	std::exit(10);
}


bool	startsWith(const std::string& toExpect, const std::string& text) {

	int start = 0;
	while (std::isspace(text[start])) {
		start++;
	}
	return (toExpect.length() > text.length() ?
		false : text.substr(start, toExpect.length()) == toExpect);
}

unsigned int countWords(const std::string& str)
{
    std::stringstream stream(str);
    return std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
}

bool isOnlySpaces(const std::string& str) {

	std::size_t i = 0;
	std::size_t len = str.length();
	while (i < len) {
		if (!isspace(str[i])) {
			return false;
		}
		i++;
	}
	return true;
}


std::string normalizeHostname(const std::string& hostname) {
	if (hostname == "localhost" || hostname == "127.0.0.1") {
		return "localhost";
	}
	return hostname;
}


void	ifstreamToString(std::ifstream& source, std::string& destination) {

	destination.assign(std::istreambuf_iterator<char>(source), std::istreambuf_iterator<char>());
}


bool	isFile(const std::string& filename) {

	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0 && !S_ISDIR(buffer.st_mode));
}


bool isDirectory(const std::string& filename) {
	struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0) {
		return false;
	}
	return S_ISDIR(buffer.st_mode);
}


std::string	basicHtml(const std::string& title, const std::string& body) {

	return (
		"<!DOCTYPE html lan=\"en\">\n"
		"<html>\n"
		"<head>\n"
			"\t<title>" + title + "</title>\n"
			"\t<style>\n"
				"\t\tbody {font-family: Arial, Helvetica, sans-serif; font-size: 16px;}\n"
			"\t</style>\n"
		"</head>\n"
		"<body>\n"
			+ body +
		"</body>\n"
		"</html>\n"
		);
}
