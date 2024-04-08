#include "SimpleSocket.hpp"

HDE::SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
    // define address struct
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = interface;
    // Establish connection
    _sock = socket(domain, service, protocol);
    testConnection(_sock);

}

HDE::SimpleSocket::~SimpleSocket()
{
}

void HDE::SimpleSocket::testConnection(int itemToTest)
{
    if (itemToTest < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "Connection successful" << std::endl;
}

struct sockaddr_in HDE::SimpleSocket::getAddress()
{
    return _address;
}

int HDE::SimpleSocket::getSocket()
{
    return _sock;
}

int HDE::SimpleSocket::getConnection()
{
    return _connection;
}

void HDE::SimpleSocket::setConnection(int conection)
{
    _connection = conection;
}