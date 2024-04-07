#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace HDE
{
    class SimpleSocket
    {
    public:
        SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
        virtual int connectToNetwork(int _sock, struct sockaddr_in _address) = 0;
        ~SimpleSocket();
        void testConnection(int itemToTest);
        struct sockaddr_in getAddress();
        int getSocket();
        int getConnection();

        void setConnection(int connection);

    private:
        struct sockaddr_in _address;
        int _sock;
        int _connection;
    };
}

#endif