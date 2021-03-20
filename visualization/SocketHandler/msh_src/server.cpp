#include "../msh.h"

int SH::Server::get_free_slot(const std::vector<sh_socket>& vec)
{
    for(size_t i=0; i < vec.size(); i++)
    {
        /*
        *   This line needs some explanation:
        *   If the pointer stores a nullptr, the slot has never been used.
        *   If the boolean where the pointer points to stores false, the slot is currently not in use.
        *   However, if we would try the access the nullptr to inquire whether the boolean is fals or not, the programm will crash.
        *   Therefore, we first check if the pointer stores a nullptr. If so, the other conditions will be ignored.
        */
        if(vec.at(i).is_processed == nullptr || !*vec.at(i).is_processed)
            return i;
    }
    return -1;
}

SH::Server::Server(void)
{
    this->handler = nullptr;
    this->running = false;
}

SH::Server::Server(SocketHandler& handler, size_t max_slots)
{
    this->handler = &handler;
    this->server_sockets.resize(max_slots);
    this->running = false;
}

SH::Server::~Server(void)
{
    this->stop();
}

void SH::Server::listen(Server* server)
{
    while(server->running)
    {
        cppsock::socket sock_buff;
        // wait for connection
        error_t accept_error = server->listener.accept(sock_buff);

        // generate errors
        error_t sockethandler_error = 0;
        int free_slot = get_free_slot(server->server_sockets);
        if(!server->handler->is_running())      sockethandler_error |= error_code_t::HANDLER_NOT_RUNNING;
        if(free_slot == -1)                     sockethandler_error |= error_code_t::SERVER_LIMIT;
        if(accept_error != 0)                   sockethandler_error |= error_code_t::SERVER_ACCEPT_ERROR;

        // call connect callback
        if(server->running && server->sock_data.connect_callback != nullptr)
            server->sock_data.connect_callback(&sock_buff, &server->sock_data.persistant, sockethandler_error);
        
        if(server->running && server->handler->is_running() && sockethandler_error == 0)
        {
            server->server_sockets.at(free_slot).sock = std::move(sock_buff);
            server->handler->add(&server->server_sockets.at(free_slot).sock, &server->sock_data, &server->server_sockets.at(free_slot).is_processed);
        }
        else
            sock_buff.close();
    }
}

error_t SH::Server::start(const char* hostname, uint16_t port, int backlog)
{
    error_t sock_error = 0;
    if(this->handler != nullptr)
    {
        if(!this->running)
        {
            // setup listener socket
            sock_error = cppsock::tcp_server_setup(this->listener, hostname, port, backlog);

            // start listner thread
            if(sock_error == 0)
            {
                this->running = true;
                this->listener_thread = thread(listen, this);
            }
        }
    }
    else
    {
        throw sockethandler_null();
    }
    return sock_error;
}

error_t SH::Server::start(const char* hostname, const char* service, int backlog)
{
    error_t sock_error = 0;
    if(this->handler != nullptr)
    {
        if(!this->running)
        {
            // setup listener socket
            sock_error = cppsock::tcp_server_setup(this->listener, hostname, service, backlog);

            // start listner thread
            if(sock_error == 0)
            {
                this->running = true;
                this->listener_thread = thread(listen, this);
            }
        }
    }
    else
    {
        throw sockethandler_null();
    }
    return sock_error;
}

void SH::Server::stop(void)
{
    if(this->running)
    {

        this->running = false;
        this->listener.close();

        // wait for listener thread to finish
        this->listener_thread.join();

        // for every socket...
        for(sh_socket& s : this->server_sockets)
        {
            // ...shutdown the connection, the socket will remain valid
            s.sock.close();

            // ...wait until the socket-handler closes the socket to become invalid
            while(s.is_processed != nullptr && *s.is_processed);
        }
    }
}

void SH::Server::set_max_slots(size_t slots) noexcept
{
    server_mutex.lock();
    this->server_sockets.resize(slots);
    server_mutex.unlock();
}

size_t SH::Server::get_used_slots(void) const noexcept
{
    size_t cntr = 0;
    for(const sh_socket& s : this->server_sockets)
    {
        if(s.is_processed != nullptr && *s.is_processed)
            ++cntr;
    }
    return cntr;
}

void SH::Server::set_callbacks(connect_func_t cc, disconnect_func_t dc, recv_func_t rc)  noexcept
{
    this->sock_data.connect_callback        = cc;
    this->sock_data.disconnect_callback     = dc;
    this->sock_data.receive_callback        = rc;
}

