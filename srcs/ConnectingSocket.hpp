#ifndef CONNECTINGSOCKET_HPP
#define CONNECTINGSOCKET_HPP

#include "SimpleSocket.hpp"

namespace HDE
{
    class ConnectingSocket : public SimpleSocket
    {
    public:
        ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface) {};
        int connectToNetwork(int _sock, struct sockaddr_in _address) override;
    };
}

#endif