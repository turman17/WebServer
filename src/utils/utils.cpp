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


bool isDirectory(std::string& filename) {
	struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0) {
		return false;
	}
	if (S_ISDIR(buffer.st_mode)) {
		if (*(filename.end() - 1) != '/') {
			filename += '/';
		}
		return (true);
	} else {
		return (false);
	}
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


char**	vectorToCharPtrArr(const std::vector<std::string>& vector) {

	char**	result = new char*[vector.size() + 1];
	int				i = 0;

	for (std::vector<std::string>::const_iterator it = vector.begin();
			it != vector.end(); it++) {
				result[i] = new char[(*it).length() + 1];
				std::memset(result[i], 0, (*it).length() + 1);
				std::strcpy(result[i], (*it).c_str());
				i++;
			}
	result[i] = NULL;
	return (result);
}


void	cleanCharPtrArr(char**& toClean) {

	for (int i = 0; toClean[i]; i++) {
		delete[] toClean[i];
	}
	delete[] toClean;
}
