#ifndef __schwarm_client_h__
#define __schwarm_client_h__

#include <atomic>
#include "my_msh.h"
#include "../SchwarmPacket/packet.h"

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <thread>
    #include <mutex>
#else
    #include <mingw.thread.h>
    #include <mingw.mutex.h>
#endif

namespace Schwarm
{
    namespace Client
    {
        struct SharedSimulationMemory
        {
            SH::Client* client{nullptr};
#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
            std::mutex sync;
#else
            mingw_stdthread::xp::mutex sync;
#endif

            std::atomic_int recv_packed_id{-1};
            std::atomic_bool start{false};
            GoalPacket goalpacket;
            ErrorPacket errorpacket;
        };

        /*
        *   This function will be called if the client connects to a server.
        *   Parameters:
        *       cppsock::socket* socket -> Pointer to the socket.
        *       void** persistant -> Pointer for additional data.
        *       error_t error -> Error flags.
        */
        void on_pathsimu_connect(cppsock::socket*, void**, error_t);

        /*
        *   This function will be called if the client disconnects from a server.
        *   Parameters:
        *       cppsock::socket* socket -> Pointer to the socket.
        *       void** persistant -> Pointer for additional data.
        */
        void on_pathsimu_disconnect(cppsock::socket*, void**);

        /*
        *   This function will be called if a packet is received.
        *   Parameters:
        *       cppsock::socket* socket -> Pointer to the socket.
        *       void** persistant -> Pointer for additional data.
        *       SH::data_channel channel -> The data channel where the packet has been received.
        */
        void on_pathsimu_receive(cppsock::socket*, void**, SH::data_channel);

        /*
        *   Processes the received packet.
        *   Parameter:
        *       uint8_t* data -> Pointer to the databuffer (bytebuffer).
        *       void** persistent -> Persistent pointer, pointer to SharedSimuMemory structure.
        */
        void process_packet(uint8_t*, void**);

        /*
        *    Function that is called within the thread.
        *    Parameters:
        *       std::atomic_bool* running -> 'true' if thread is running, 'false' if thread is not running.
        *       const std::string* imagefolfer -> Path to image folder, needed for server.
        */
        void run_pathserver(std::atomic_bool*, const std::string*);

        /*
        *   Starts the server asynchronously.
        *   Parameters:
        *       std::atomic_bool* running -> 'true' if thread is running, 'false' if thread is not running.
        *       const std::string* imagefolfer -> Path to image folder, needed for server.
        */
        void start_pathserver(std::atomic_bool*, const std::string*);

        /*
        *   Stopps the server and waits until the thread has finished.
        *   Parameters:
        *       std::atomic_bool* running -> Requiered to wait for the server.
        */
        void shutdown_pathserver(std::atomic_bool*, Client::SharedSimulationMemory&);
    };
};

#endif // __schwarm_client_h__
