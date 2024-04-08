#include "BindingSocket.hpp"

HDE::BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface) 
{
    setConnection(connectToNetwork(getSocket(), getAddress()));
    testConnection(getConnection());
};


int HDE::BindingSocket::connectToNetwork(int _sock, struct sockaddr_in _address)
{
    return bind(_sock, (struct sockaddr *)&_address, sizeof(_address));
}

