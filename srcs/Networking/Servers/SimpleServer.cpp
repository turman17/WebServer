#include "SimpleServer.hpp"

HDE::SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog)
{
    _socket = new ListeningSocket(domain, service, protocol, port, interface, backlog);

}


HDE::ListeningSocket * HDE::SimpleServer::getSocket()
{
    return _socket;
}