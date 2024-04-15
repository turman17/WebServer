#ifndef TESTSERVER_HPP
#define TESTSERVER_HPP

#include "SimpleServer.hpp"

namespace HDE
{
    class TestServer : public SimpleServer
    {
    public:
        void launch();
        TestServer();
        int newSocket;
    private:

        char buffer[30000] = {0};
        void _accepter();
        void _handler();
        void _responder();
    };
}

#endif