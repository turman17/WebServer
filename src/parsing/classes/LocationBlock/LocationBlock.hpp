#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

# include "stdlibraries.hpp"

class LocationBlock {
public:

private:
	std::string					m_routePath = "";
	std::string					m_root = "";
	std::string					m_indexFile = "";
	std::string					m_uploadedFiles = "";
	std::pair<int, std::string> m_redirection = {0, ""};
	std::vector<std::string>	m_allowedMethods = {"GET", "POST", "DELETE"};
	bool						m_directoryListing = false;
	
};

#endif
