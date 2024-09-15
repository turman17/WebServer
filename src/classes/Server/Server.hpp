#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"
#include <exception>

class Server
{
public:
	Server();
	~Server();
	void run();
	void loadConfig(const char *filename);
	bool getParsingOutput();

	//* tmp
	void printSettings();
	//* end of tmp
	class BadOpenFile : public std::exception
	{
	public:
		const char *what() const throw();
	};

	class UnknownDirective : public std::exception
	{
	public:
		const char *what() const throw();
	};

	class BadConfig : public std::exception
	{
	public:
		const char *what() const throw();
	};

private:
	std::vector<ServerBlock> m_serverBlocks;
	std::vector<ServerSocket> m_listeningSockets;
	EventPoll m_eventsManager;
	Clients m_clientsMap;
	bool m_parsingOutput;

	void assignServerBlockSetting(const std::string &line, ServerBlock &serverBlock);
	void assignLocationBlockSetting(const std::string &line, LocationBlock &serverBlock);
	void setupListeningSockets();
	void setupEpoll();
	void acceptNewConnection(const Event &event);
};

#endif