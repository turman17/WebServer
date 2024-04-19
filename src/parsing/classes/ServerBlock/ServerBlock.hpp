#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

# include "stdlibraries.hpp"
# include "../LocationBlock/LocationBlock.hpp"

class ServerBlock {
public:
private:
	int			m_port = 80;
	int			m_maxBodySize = 1024;
	std::string	m_hostname = "0.0.0.0";
	std::string	m_serverName = "_";
	std::string	m_root = "./";
	std::string m_errorPage = "./Errors/DefaultError.html";
	std::vector<LocationBlock>	m_LocationBlocks;

};

#endif
