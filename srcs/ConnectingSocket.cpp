#include "ConnectingSocket.hpp"

HDE::ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
{
    setConnection(connectToNetwork(getSocket(), getAddress()));
    testConnection(getConnection());
};

int HDE::ConnectingSocket::connectToNetwork(int _sock, struct sockaddr_in _address)
{
    return connect(_sock, (struct sockaddr *)&_address, sizeof(_address));
}
