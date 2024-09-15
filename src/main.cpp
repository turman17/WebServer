#include "webserv.hpp"

using namespace epoll;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream fileCheck(argv[1]);
    if (!fileCheck)
    {
        std::cerr << "Error: File " << argv[1] << " does not exist or cannot be opened." << std::endl;
        return EXIT_FAILURE;
    }

    s_sigaction act;
    Server server;

    act.sa_handler = signalHandler;
    signal(SIGTSTP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

    try
    {
        server.loadConfig(argv[1]);
        if (server.getParsingOutput() == true)
        {
            server.run();
        }
    }
    catch (const Server::BadOpenFile &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const Server::BadConfig &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const Server::UnknownDirective &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}