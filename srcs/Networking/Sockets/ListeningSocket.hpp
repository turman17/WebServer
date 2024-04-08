#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "BindingSocket.hpp"

namespace HDE
{
    class ListeningSocket : public BindingSocket
    {
    public:
        ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog);
        void startListening();
    private:
        int _listening;
        int _backlog;
    };
}

#endif