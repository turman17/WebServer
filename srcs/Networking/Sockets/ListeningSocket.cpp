#include "ListeningSocket.hpp"

HDE::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : 
BindingSocket(domain, service, protocol, port, interface), _backlog(backlog)
{
    startListening();
    testConnection(getConnection());
}

void HDE::ListeningSocket::startListening()
{
    _listening = listen(getSocket(), _backlog);
    testConnection(_listening);
}
