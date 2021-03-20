#include <iostream>
#include "msh.h"
#include <windows.h>
#include <chrono>

//#define SERVER

void on_connect(cppsock::socket* socket, void** persistant, error_t error);
void on_disconnect(cppsock::socket* socket, void** persistant);
void on_recv(cppsock::socket* socket, void** persistant, SH::data_channel channel);

void on_connect(cppsock::socket* socket, void** persistant, error_t error)
{
    static std::atomic_int* counter = new std::atomic_int(0);
    *persistant = counter;

    const char msg_welcome[] = "Hello you are connected.\n";
    const char msg_error[] = "You could't connect.\n";

    if(error == SH::error_code_t::HANDLER_NO_ERROR)
    {
        cppsock::socketaddr addr;
        socket->getpeername(addr);
        socket->send(msg_welcome, sizeof(msg_welcome), 0);
        std::cout << "Client connected." << std::endl;
        std::cout << "Remote-address: " << addr.get_addr() << ":" << addr.get_port() << std::endl;
    }
    if(error & SH::error_code_t::HANDLER_NOT_RUNNING)
    {
        std::cout << "[ERROR] Handler is not running." << std::endl;
        socket->send(msg_error, sizeof(msg_error), 0);
    }
#ifdef SERVER
    if(error & SH::error_code_t::SERVER_LIMIT)
    {
        std::cout << "[ERROR] Server is full." << std::endl;
        socket->send(msg_error, sizeof(msg_error), 0);
    }
    if(error & SH::error_code_t::SERVER_ACCEPT_ERROR)
    {
        std::cout << "[ERROR] Accept error." << std::endl;
    }
#else
    if(error & SH::error_code_t::CLIENT_CONNECT_ERROR)
    {
        std::cout << "[ERROR] Connect error." << std::endl;
    }
#endif
}

void on_disconnect(cppsock::socket* socket, void** persistant)
{
    cppsock::socketaddr addr;
    socket->getpeername(addr);
    std::cout << "Client disconnected." << std::endl;
    std::cout << "Remote-address: " << addr.get_addr() << ":" << addr.get_port() << std::endl;
}

void on_recv(cppsock::socket* socket, void** persistant, SH::data_channel channel)
{
    if(channel == SH::data_channel::DATA_CHANNEL_STD)
    {
        cppsock::socketaddr addr;
        socket->getpeername(addr);
        char textbuff[256];
        size_t n_bytes = socket->recv(textbuff, sizeof(textbuff), 0);
        textbuff[n_bytes] = '\0';
        std::cout << "--- MSG NUMBER: " << (*((int*)*persistant))++ << " ---" << std::endl;
        std::cout << "[" << addr.get_addr() << ":" << addr.get_port() << "] >> " << textbuff << std::endl;
        socket->send(textbuff, n_bytes, 0);
    }
}

int main()
{
    SH::SocketHandler handler;
    handler.start();
    std::cout << "Socket-handler started." << std::endl;

#ifdef SERVER
    SH::Server server(handler, 2);
    server.set_callbacks(on_connect, on_disconnect, on_recv);
    server.start("127.0.0.1", 10000, 1);
    std::cout << "Started server." << std::endl;
#else
    SH::Client client(handler);
    client.set_callbacks(on_connect, on_disconnect, on_recv);
    error_t err = client.connect("127.0.0.1", 10000);
    if(err != 0)
    {
        std::cout << "Cant connect!" << std::endl;
        return -1;
    }
    std::cout << "Client connected!" << std::endl;
#endif

    bool running = true;
    while(running)
    {
#ifndef SERVER  
        /*
        std::string in;
        std::getline(std::cin, in);
        client.get_socket().send(in.c_str(), in.size(), 0);
        */
#endif
        running = !GetAsyncKeyState(VK_ESCAPE);
    }

#ifdef SERVER
    server.stop();
    std::cout << "Stopped server." << std::endl;
    delete((int*)*server.persist_ptr());
#else
    client.disconnect();
    std::cout << "Disconnected!" << std::endl;
    delete((int*)*client.ersist_ptr());
#endif

    handler.stop();
    std::cout << "Stopped handler." << std::endl;

    return 0;
}