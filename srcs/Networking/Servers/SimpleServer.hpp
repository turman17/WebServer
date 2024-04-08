#ifndef SIMPLESERVER_HPP
#define SIMPLESERVER_HPP

#include "../HdelibsNetworking.hpp"

namespace HDE
{
    class SimpleServer
    {
    public:
        SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog);
        virtual void launch() = 0;
        ListeningSocket *getSocket();
    private:
        ListeningSocket *_socket;
        virtual void _accepter() = 0;
        virtual void _handler() = 0;
        virtual void _responder() = 0;
    };
} // namespace HDE

#endif