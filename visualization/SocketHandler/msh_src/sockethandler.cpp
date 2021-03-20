#include "../msh.h"
#include <chrono>

SH::SocketHandler::SocketHandler(void)
{
    this->running = false;
}

SH::SocketHandler::~SocketHandler(void)
{
    this->stop();
}

void SH::SocketHandler::handle(SH::SocketHandler* handler, cppsock::socket* socket, data_channel channel)
{
    socket_thread_info* thread_info = &handler->socket2thread_info[socket];
    socket_data* sock_data = handler->socket2data[socket];

    if(channel == data_channel::DATA_CHANNEL_STD)
    {
        thread_info->socket_thread_oob = thread(handle, handler, socket, data_channel::DATA_CHANNEL_OOB);
    }

    while(thread_info->running)
    {
        uint8_t indicator;
        if(socket->recv(&indicator, 1, channel | MSG_PEEK) > 0)
        {
            if(sock_data->receive_callback != nullptr)
                sock_data->receive_callback(socket, &sock_data->persistant, channel);
        }
        else
        {
            thread_info->running = false;
        }
    }

    if(channel == data_channel::DATA_CHANNEL_STD)
    {
        thread_info->socket_thread_oob.join();
        if(sock_data->disconnect_callback != nullptr)
            sock_data->disconnect_callback(socket, &sock_data->persistant);
        if(socket->is_valid())
            socket->close();
        handler->handler_mutex.lock();
        handler->socket2data.erase(socket);
        handler->socket2thread_info.erase(socket);
        handler->handler_mutex.unlock();
        thread_info->still_running = false;
    }
}

void SH::SocketHandler::start(void)
{
    if(!this->running)
        this->running = true;
}

void SH::SocketHandler::stop(void)
{
    if(this->running)
    {
        this->running = false;
        for(auto iter = this->socket2thread_info.begin(); iter != this->socket2thread_info.end(); iter++)
        {
            iter->second.running = false;
            iter->first->close();
            while(iter->first->is_valid());
        }
    }
}

void SH::SocketHandler::add(cppsock::socket* socket, SH::socket_data* sock_data, std::atomic_bool** ret_processed)
{
    if(this->running)
    {
        // new memory will be allocated for both maps, this may be a critical operation
        this->handler_mutex.lock();
        this->socket2data[socket] = sock_data;
        this->socket2thread_info[socket].running = true;
        this->handler_mutex.unlock();
        this->socket2thread_info[socket].still_running = true;
        this->socket2thread_info[socket].socket_thread_std = thread(handle, this, socket, data_channel::DATA_CHANNEL_STD);
        this->socket2thread_info[socket].socket_thread_std.detach();
        *ret_processed = &this->socket2thread_info[socket].still_running;
    }
    else
    {
        throw sockethandler_invalid_operation();
    }
    // use std functions where possible
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}