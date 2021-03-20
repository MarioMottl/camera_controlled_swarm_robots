#include "client.h"
#include <cstdio>
#include <iostream>
#include <system_error>
#include "../includes/my_msg.h"
#include "../SchwarmPacket/packet.h"

using namespace Schwarm;

void Client::on_pathsimu_connect(cppsock::socket* socket, void** persistent, error_t error)
{
    // no error case
    if(error == SH::error_code_t::HANDLER_NO_ERROR) 
    {
        std::cout << get_msg("INFO / CLIENT") << "Successfully connected." << std::endl;
        std::cout << get_msg("INFO / CLIENT") << "Device-Address: " << socket->getsockname().get_addr() << ":" << socket->getsockname().get_port() << std::endl;
        std::cout << get_msg("INFO / CLIENT") << "Remote-Address: " << socket->getpeername().get_addr() << ":" << socket->getsockname().get_port() << std::endl;
    }
    else    // error case
    {
        std::cout << get_msg("ERROR / CLIENT") << "Can't connect to server." << std::endl;
        std::cout << get_msg("INFO / CLIENT") << "Errorcode: " << error << std::endl;
    }
}

void Client::on_pathsimu_disconnect(cppsock::socket* socket, void** persistent)
{
    std::cout << get_msg("INFO / CLIENT") << "Diconnected." << std::endl;
}

void Client::on_pathsimu_receive(cppsock::socket* socket, void** persistent, SH::data_channel channel)
{
    uint8_t buff1[5];
    /*
    *   For the first time only read the first 5 bytes from the the buffer and only peek it.
    *   Because of the peek flag those bytes bytes will not be deleted from the buffer.
    *   This is done to get the size of the whole packet without modifying the receive buffer.
    */
    socket->recv(buff1, sizeof(buff1), channel | MSG_PEEK);
    const size_t* packet_size = Packet::size_ptr(buff1);    // Get size of the packet.

    uint8_t buff2[*packet_size];                            // Create a second buffer with the size of the packet.
    /*
    *   Receive the second time with the full size of the packet and without peeking so that the
    *   buffer gets cleared after reading the data from it.
    */
    socket->recv(buff2, sizeof(buff2), channel);

    GoalPacket packet;
    packet.allocate(*packet_size);
    packet.set(buff2);
    packet.decode();
    std::cout << "X: " << packet.get_goal_x() << "  Y: " << packet.get_goal_y() << std::endl;

    // Process the packet...
    //process_packet(buff2, persistent);
}

void Client::process_packet(uint8_t* buff, void** persistent)
{
    SharedSimulationMemory* mem = (SharedSimulationMemory*)*persistent;

    const uint8_t* id = Packet::id_ptr(buff);       // get id of packet
    const size_t* size = Packet::size_ptr(buff);    // get size of packet

    if(*id == AcnPacket::PACKET_ID)
    {
        std::cout << get_msg("INFO / CLIENT") << "Successfully generated path." << std::endl;
    }
    else if(*id == ErrorPacket::PACKET_ID)
    {
        if(mem != nullptr && mem->recv_packed_id == -1)
        {
            mem->sync.lock();
            mem->errorpacket.allocate(*size);
            mem->errorpacket.set(buff);
            mem->errorpacket.decode();
            mem->sync.unlock();
            mem->recv_packed_id = *id;
        }
    }
    else if(*id == GoalPacket::PACKET_ID)
    {
        if(mem != nullptr && mem->recv_packed_id == -1)
        {  
            mem->sync.lock();
            mem->goalpacket.allocate(*size);
            mem->goalpacket.set(buff);
            mem->goalpacket.decode();
            mem->sync.unlock();
            mem->recv_packed_id = *id;
        }
    }
}

void Client::run_pathserver(std::atomic_bool* running, const std::string* imgfolder)
{
    *running = true;
    char cmd[256];
    sprintf(cmd, "path_server.exe.lnk %s", imgfolder->c_str());  // Generate command to call the server.
    int ret = 0;
    if((ret = std::system(cmd)) < 0)    // Start server
    {
        std::cout << get_msg("ERROR / SERVER") << "Server closed with code: " << ret << " / " << std::hex << ret << std::endl;
        throw std::system_error();  // System error if server is unable to start.
    }
    *running = false;
}

void Client::start_pathserver(std::atomic_bool* running, const std::string* imgfolder)
{
#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    std::thread serverthread = std::thread(Client::run_pathserver, running, imgfolder);
#else
    mingw_stdthread::thread serverthread = mingw_stdthread::thread(Client::run_pathserver, running, imgfolder);   
#endif
    serverthread.detach();
    while(!*running);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void Client::shutdown_pathserver(std::atomic_bool* running, Client::SharedSimulationMemory& sharedsimumem)
{
    // Compile exit packet.
    ExitPacket exit;
    exit.allocate(exit.min_size());
    exit.encode();

    const std::string& remoteaddress = sharedsimumem.client->get_socket().getpeername().get_addr();
    uint16_t remoteport = sharedsimumem.client->get_socket().getpeername().get_port();
    
    // If client is not connected to pathserver...
    if(!(remoteaddress == Schwarm::SIMU_SERVER_ADDR && remoteport == Schwarm::SIMU_SERVER_PORT))
    {
        // Disconnect from current server.
        sharedsimumem.client->disconnect();
        // Connect to pathserver.
        sharedsimumem.client->connect(Schwarm::SIMU_SERVER_ADDR, Schwarm::SIMU_SERVER_PORT);
    }

    // Send exit packet to pathserver.
    sharedsimumem.client->get_socket().send(exit.rawdata(), exit.size(), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    sharedsimumem.client->disconnect();
    // Wait until server is stopped.
    while(*running);
}   