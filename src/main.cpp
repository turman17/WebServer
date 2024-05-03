#include "webserv.hpp"

using namespace epoll;

int main(void) {

	s_sigaction	act;
	Server		server;

	act.sa_handler = signalHandler;
	signal(SIGTSTP, SIG_IGN);
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);

	server.loadConfig("webserv.conf");
	//server.printSettings();
	server.run();

}
