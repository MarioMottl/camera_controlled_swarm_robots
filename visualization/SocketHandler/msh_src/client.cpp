#include "../msh.h"

SH::Client::Client(void)
{
    this->handler = nullptr;
    this->__is_connected = false;
}

SH::Client::Client(SocketHandler& handler)
{
    this->handler = &handler;
    this->__is_connected = false;
}

SH::Client::~Client(void)
{
    this->disconnect();
}

error_t SH::Client::connect(const char* hostname, uint16_t port)
{
    error_t sock_error = 0;
    if(this->handler != nullptr)
    {
        // only connect if socket is not valid, in other words if it is not connected to any server
        if(!this->is_connected())
        {
            // try to connect
            sock_error = cppsock::tcp_client_connect(this->client_socket.sock, hostname, port);

            // generate errors
            error_t sockethandler_error = 0;
            if(!this->handler->is_running())    sockethandler_error |= error_code_t::HANDLER_NOT_RUNNING;
            if(sock_error != 0)                 sockethandler_error |= error_code_t::CLIENT_CONNECT_ERROR;

            // call connect callback
            if(this->sock_data.connect_callback != nullptr)
                this->sock_data.connect_callback(&this->client_socket.sock, &this->sock_data.persistant, sockethandler_error);
            
            if(sockethandler_error == 0)
            {
                this->__is_connected = true;
                this->handler->add(&this->client_socket.sock, &this->sock_data, &this->client_socket.is_processed);
            }
            else    // close socket if error has occured
                this->client_socket.sock.close();
        }
    }
    else
    {
        throw sockethandler_null();
    }
    return sock_error;
}

error_t SH::Client::connect(const char* hostname, const char* service)
{
    error_t sock_error = 0;
    if(this->handler != nullptr)
    {
        // only connect if socket is not valid, in other words if it is not connected to any server
        if(!this->is_connected())
        {
            // try to connect
            sock_error = cppsock::tcp_client_connect(this->client_socket.sock, hostname, service);

            // generate errors
            error_t sockethandler_error = 0;
            if(!this->handler->is_running())    sockethandler_error |= error_code_t::HANDLER_NOT_RUNNING;
            if(sock_error != 0)                 sockethandler_error |= error_code_t::CLIENT_CONNECT_ERROR;

            // call connect callback
            if(this->sock_data.connect_callback != nullptr)
                this->sock_data.connect_callback(&this->client_socket.sock, &this->sock_data.persistant, sockethandler_error);
            
            if(sockethandler_error == 0)
            {
                this->__is_connected = true;
                this->handler->add(&this->client_socket.sock, &this->sock_data, &this->client_socket.is_processed);
            }
            else    // close socket if error has occured
                this->client_socket.sock.close();
        }
    }
    else
    {
        throw sockethandler_null();
    }
    return sock_error;
}

void SH::Client::disconnect(void)
{
    if(this->is_connected())
    {
        this->client_socket.sock.close();

        // wait until the socket-handler closes the socket to become invalid
        while(this->client_socket.is_processed != nullptr && *this->client_socket.is_processed);
        this->__is_connected = false;
    }
}

void SH::Client::set_callbacks(connect_func_t cc, disconnect_func_t dc, recv_func_t rc)  noexcept
{
    this->sock_data.connect_callback        = cc;
    this->sock_data.disconnect_callback     = dc;
    this->sock_data.receive_callback        = rc;
}