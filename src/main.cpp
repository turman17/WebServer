#include "webserv.hpp"

using namespace epoll;

int main(int argc, char**argv) {

	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		exit(1);
	}

	s_sigaction	act;
	Server		server;

	act.sa_handler = signalHandler;
	signal(SIGTSTP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);

	server.loadConfig(argv[1]);
	//server.printSettings();
	server.run();
}
