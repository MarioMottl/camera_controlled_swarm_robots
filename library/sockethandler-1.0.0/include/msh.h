/******************************************************************************************************************************************
* Title:        Socket-Handler
* Author:       Michael Reim
* Date:         12.11.2020
* Description:
*   Library for easy implementation of server - client - communication via sockets.
*   Multithreaded handling of multiple sockets is also supportet.
*
* Used librarys:
*   - C++ standard libarys
*   - libws2_32.a (for windows)
*   - libcppsock.a from @Clemens Pruggmayer
*
* Librarys and files to link:
*   - libws2_32.a
*   - libcppsock.a
*   - cppsock_winonly.cpp
*
* @version release 1.0.0
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* If there are any bugs, contact me!
******************************************************************************************************************************************/

#ifndef __msh_h__
#define __msh_h__

#include <cppsock.hpp>
#include <atomic>
#include <vector>
#include <map>
#include <exception>

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <thread>
    #include <mutex>
    using std::thread;
    using std::mutex;
#else
    #include <mingw.thread.h>
    #include <mingw.mutex.h>
    using mingw_stdthread::thread;
    using mingw_stdthread::mutex;
#endif 

#ifndef error_t
    using error_t = int;
#endif

namespace SH
{
    class sockethandler_null : public std::exception
    {
    public:
        sockethandler_null(void) = default;
        virtual const char* what(void) const throw()
        {
            return "Can not use \'nullptr\' das socket-handler.";
        }
    };

    class sockethandler_invalid_operation : public std::exception
    {
    public:
        sockethandler_invalid_operation(void) = default;
        virtual const char* what(void) const throw()
        {
            return "\nFollowing operations will throw this exception:\n\tCalling SocketHandler::add() while socket-handler is not running.";
        }
    };

    enum data_channel : int
    {
        DATA_CHANNEL_STD = 0,
        DATA_CHANNEL_OOB = MSG_OOB
    };

    enum error_code_t : int
    {
        HANDLER_NO_ERROR        = 0x0,
        HANDLER_NOT_RUNNING     = 0x1,
        SERVER_LIMIT            = 0x2,
        SERVER_ACCEPT_ERROR     = 0x4,
        CLIENT_CONNECT_ERROR    = 0x8
    };

    typedef void (*connect_func_t)      (cppsock::socket*, void**, error_t);
    typedef void (*disconnect_func_t)   (cppsock::socket*, void**);
    typedef void (*recv_func_t)         (cppsock::socket*, void**, data_channel);

    struct sh_socket
    {
        cppsock::socket sock;
        std::atomic_bool* is_processed{nullptr};
    };

    struct socket_data
    {
        connect_func_t connect_callback{nullptr};
        disconnect_func_t disconnect_callback{nullptr};
        recv_func_t receive_callback{nullptr};
        void* persistant{nullptr};
    };

    struct socket_thread_info
    {
        thread socket_thread_std, socket_thread_oob;
        std::atomic_bool running{false};
        std::atomic_bool still_running{false};
    };


    class SocketHandler
    {
    private:
        std::map<cppsock::socket*, socket_data*> socket2data;
        std::map<cppsock::socket*, socket_thread_info> socket2thread_info;
        mutex handler_mutex;
        std::atomic_bool running;

        static void handle(SocketHandler*, cppsock::socket*, data_channel);

    public:
        SocketHandler(void);

        SocketHandler(const SocketHandler&) = delete;
        SocketHandler& operator= (const SocketHandler&) = delete;

        SocketHandler(SocketHandler&&) = delete;
        SocketHandler& operator= (SocketHandler&&) = delete;

        virtual ~SocketHandler(void);

        void start(void);
        void stop(void);
        void add(cppsock::socket*, socket_data*, std::atomic_bool**);

        size_t  size(void)                              const noexcept  {return this->socket2data.size();}
        void**  persist_ptr(cppsock::socket& socket)    noexcept        {return &this->socket2data[&socket]->persistant;}
        bool    is_running(void)                        const noexcept  {return this->running;}
    };

    class Server
    {
    private:
        SocketHandler* handler;
        cppsock::socket listener;
        std::vector<sh_socket> server_sockets;
        thread listener_thread;
        mutex server_mutex;
        socket_data sock_data;
        std::atomic_bool running;

        static void listen(Server*);
        static int get_free_slot(const std::vector<sh_socket>&);

    public:
        Server(void);
        Server(SocketHandler& handler, size_t max_slots = 0);

        Server(const Server&) = delete;
        Server& operator= (const Server&) = delete;

        Server(Server&) = delete;
        Server& operator= (Server&) = delete;

        virtual ~Server(void);

        error_t start(const char* hostname, uint16_t port, int backlog);
        error_t start(const char* hostname, const char* service, int backlog);
        void    stop(void);

        size_t  get_used_slots(void)        const noexcept;
        size_t  get_max_slots(void)         const noexcept  {return this->server_sockets.size();}
        void    set_max_slots(size_t slots) noexcept;
        
        void set_sockethandler(SocketHandler& handler)                      noexcept    {this->handler = &handler;}
        void set_callbacks(connect_func_t, disconnect_func_t, recv_func_t)  noexcept;
        void set_connect_callback(connect_func_t cc)                        noexcept    {this->sock_data.connect_callback       = cc;}
        void set_disconnect_callback(disconnect_func_t dc)                  noexcept    {this->sock_data.disconnect_callback    = dc;}
        void set_receive_callback(recv_func_t rc)                           noexcept    {this->sock_data.receive_callback       = rc;}

        cppsock::socket& get_socket(int idx)    noexcept        {return this->server_sockets.at(idx).sock;}
        void**  persist_ptr(void)               noexcept        {return &this->sock_data.persistant;}
        bool    is_running(void)                const noexcept  {return this->running;}
    };

    class Client
    {
    private:
        SocketHandler* handler;
        sh_socket client_socket;
        socket_data sock_data;
        std::atomic_bool __is_connected;

    public:
        Client(void);
        Client(SocketHandler& handler);

        Client(const Client&) = delete;
        Client& operator= (const Client&) = delete;

        Client(Client&) = delete;
        Client& operator= (Client&) = delete;

        virtual ~Client(void);

        error_t connect(const char* hostname, uint16_t port);
        error_t connect(const char* hostname, const char* service);
        void    disconnect(void);

        void set_sockethandler(SocketHandler& handler)                      noexcept    {this->handler = &handler;}
        void set_callbacks(connect_func_t, disconnect_func_t, recv_func_t)  noexcept;
        void set_connect_callback(connect_func_t cc)                        noexcept    {this->sock_data.connect_callback      = cc;}
        void set_disconnect_callback(disconnect_func_t dc)                  noexcept    {this->sock_data.disconnect_callback   = dc;}
        void set_receive_callback(recv_func_t rc)                           noexcept    {this->sock_data.receive_callback      = rc;}

        cppsock::socket& get_socket(void)   noexcept        {return client_socket.sock;}
        void**  ersist_ptr(void)            noexcept        {return &this->sock_data.persistant;}
        bool    is_connected(void)          const noexcept  {return __is_connected;}
    };
};

#endif // __msh_h__