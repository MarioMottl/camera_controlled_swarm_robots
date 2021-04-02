#ifndef __schwarm_client_h__
#define __schwarm_client_h__

#include "../SchwarmPacket/packet.h"
#include <atomic>
#include <thread>
#include <mutex>
#include <cppsock.hpp>

namespace Schwarm
{

    constexpr char PATH_SERVER_ADDR[] = "127.0.0.1";
    constexpr uint16_t PATH_SERVER_PORT = 10000;
    constexpr char DETECTION_SERVER_ADDR[] = "10.212.134.5";
    constexpr uint16_t DETECTION_SERVER_PORT = 10001;
    constexpr char CONTROL_SERVER_ADDR[] = "10.212.134.5";
    constexpr uint16_t CONTROL_SERVER_PORT = 10002;

    namespace Client
    {
        enum ClientType
        {
            PATH_SERVER,
            DETECTION_SERVER,
            CONTROL_SERVER,
            GENERAL
        };

        struct DetecCoord
        {
            float x, y;
        };

        struct SharedMemory
        {
            std::shared_ptr<cppsock::tcp::socket> client;

            // general variables
            std::mutex sync;
            std::atomic_bool start{ false };
            std::atomic_bool real{ false };
            void* vehicles{ nullptr };          // cant use vehicle-buffer-pointer 

            // used for PATH_SERVER
            std::atomic_int recv_packed_id{-1};
            GoalPacket goalpacket;
            ErrorPacket errorpacket;

            // only used for detection
            std::map<uint8_t, DetecCoord> detec_coords;
        };

        /*
        *   This function will be called if the client connects to a server.
        *   Parameters:
        *       cppsock::socket* socket -> Pointer to the socket.
        *       void** persistant -> Pointer for additional data.
        *       error_t error -> Error flags.
        */
        void on_connect(std::shared_ptr<cppsock::tcp::socket> socket, cppsock::socketaddr_pair addr, void** persistent);

        /*
        *   This function will be called if the client disconnects from a server.
        *   Parameters:
        *       cppsock::socket* socket -> Pointer to the socket.
        *       void** persistant -> Pointer for additional data.
        */
        void on_disconnect(std::shared_ptr<cppsock::tcp::socket> socket, cppsock::socketaddr_pair addr, void** persistent);

        /*
        *   This function will be called if a packet is received.
        *   Parameters:
        *       cppsock::socket* socket -> Pointer to the socket.
        *       void** persistant -> Pointer for additional data.
        *       SH::data_channel channel -> The data channel where the packet has been received.
        */
        void on_path_receive(std::shared_ptr<cppsock::tcp::socket> socket, cppsock::socketaddr_pair addr, void** persistent);
        void on_detection_receive(std::shared_ptr<cppsock::tcp::socket> socket, cppsock::socketaddr_pair addr, void** persistent);

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
        void shutdown_pathserver(std::atomic_bool*, Client::SharedMemory&);
    };
};

#endif // __schwarm_client_h__
