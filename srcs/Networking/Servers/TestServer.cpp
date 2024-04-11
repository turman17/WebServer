#include "TestServer.hpp"
#include "unistd.h"
#include <cstring>
#include <string>

HDE::TestServer::TestServer()
: SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    launch();
}

void HDE::TestServer::_accepter()
{
    struct sockaddr_in address = getSocket()->getAddress();
    int addrlen = sizeof(address);
    newSocket = accept(getSocket()->getSocket(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    read(newSocket, buffer, 30000);
}

void HDE::TestServer::_handler()
{
    std::cout << buffer << std::endl;
}

void HDE::TestServer::_responder()
{
    std::string hello = "Hello from Server\n";
    write(newSocket, hello.c_str(), hello.length());
    close(newSocket);
}

void HDE::TestServer::launch()
{
    while(true)
    {
        std::cout << "______________ Waiting ______________\n";
        _accepter();
        _handler();
        _responder();
        std::cout << "______________   Done  ______________\n";
    }
}
