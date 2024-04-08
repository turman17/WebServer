#ifndef BINDINGSOCKET_HPP
#define BINDINGSOCKET_HPP

#include "SimpleSocket.hpp"

namespace HDE
{
    class BindingSocket : public SimpleSocket
    {
    public:
        BindingSocket(int domain, int service, int protocol, int port, u_long interface);
        int connectToNetwork(int _sock, struct sockaddr_in _address) override;
    };
}

#endif