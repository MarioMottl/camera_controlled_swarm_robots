#include "client.h"
#include <iostream>
#include <sstream>
#include "../includes/my_msg.h"
#include "../SchwarmPacket/packet.h"

using namespace Schwarm;

void Client::on_connect(std::shared_ptr<cppsock::tcp::socket> socket, cppsock::socketaddr_pair addr, void** persistent)
{
    std::cout << get_msg("INFO / CLIENT") << "Successfully connected to Server." << std::endl;
    std::cout << get_msg("INFO / CLIENT") << "Device-Address: " << addr.local << std::endl;
    std::cout << get_msg("INFO / CLIENT") << "Remote-Address: " << addr.remote << std::endl;
}

void Client::on_disconnect(std::shared_ptr<cppsock::tcp::socket> socket, cppsock::socketaddr_pair addr, void** persistent)
{
    std::cout << get_msg("INFO / CLIENT") << "Server disconnected." << std::endl;
    std::cout << get_msg("INFO / CLIENT") << "Device-Address: " << addr.local << std::endl;
    std::cout << get_msg("INFO / CLIENT") << "Remote-Address: " << addr.remote << std::endl;;
}

void Client::on_path_receive(std::shared_ptr<cppsock::tcp::socket> socket, cppsock::socketaddr_pair addr, void** persistent)
{
    uint8_t buff1[5];
    /*
    *   For the first time only read the first 5 bytes from the the buffer and only peek it.
    *   Because of the peek flag those bytes bytes will not be deleted from the buffer.
    *   This is done to get the size of the whole packet without modifying the receive buffer.
    */
    std::streamsize ret = socket->recv(buff1, sizeof(buff1), cppsock::peek);
    if (ret < 0)
        return;

    const uint32_t* packet_size = Packet::size_ptr(buff1);    // Get size of the packet.

    if (*packet_size > 100)
        return;

    uint8_t buff2[*packet_size];                            // Create a second buffer with the size of the packet.
    /*
    *   Receive the second time with the full size of the packet and without peeking so that the
    *   buffer gets cleared after reading the data from it.
    */
    socket->recv(buff2, sizeof(buff2), cppsock::waitall); // internal compiler error: bug report

    // Process the packet...
    process_packet(buff2, persistent);
}

void Client::on_detection_receive(std::shared_ptr<cppsock::tcp::socket> socket, cppsock::socketaddr_pair addr, void** persistent)
{
    std::map<Schwarm::Client::ClientType, Schwarm::Client::SharedMemory>* mem = (std::map<Schwarm::Client::ClientType, Schwarm::Client::SharedMemory>*) * persistent;

    uint8_t buff1[5];
    /*
    *   For the first time only read the first 5 bytes from the the buffer and only peek it.
    *   Because of the peek flag those bytes bytes will not be deleted from the buffer.
    *   This is done to get the size of the whole packet without modifying the receive buffer.
    */
    std::streamsize ret = socket->recv(buff1, sizeof(buff1), cppsock::peek);
    if (ret < 0)
        return;

    const uint32_t* packet_size = Packet::size_ptr(buff1);      // Get size of the packet.

    if (*packet_size > 100)
        return;

    uint8_t buff2[*packet_size];                                // Create a second buffer with the size of the packet.
    /*
    *   Receive the second time with the full size of the packet and without peeking so that the
    *   buffer gets cleared after reading the data from it.
    */
    socket->recv(buff2, sizeof(buff2), cppsock::waitall);       // internal compiler error: bug report

    // process detection packet
    uint8_t id = *Packet::id_ptr(buff2);
    uint32_t size = *Packet::size_ptr(buff2);

    if (id == GoalPacket::PACKET_ID && mem != nullptr)
    {
        GoalPacket detec;
        detec.allocate(size);
        detec.set(buff2);
        detec.decode();

        (*mem)[GENERAL].sync.lock();
        (*mem)[DETECTION_SERVER].detec_coords[detec.get_vehicle_id()] = { detec.get_goal_x(), detec.get_goal_y() };
        (*mem)[GENERAL].sync.unlock();
        (*mem)[DETECTION_SERVER].recv_packed_id = id;

        //std::cout << "ID: " << detec.get_vehicle_id() << " X: " << detec.get_goal_x() << " Y: " << detec.get_goal_y() << std::endl;
    }
}

void Client::process_packet(uint8_t* buff, void** persistent)
{
    std::map<Schwarm::Client::ClientType, Schwarm::Client::SharedMemory>* mem = (std::map<Schwarm::Client::ClientType, Schwarm::Client::SharedMemory>*)*persistent;

    const uint8_t* id = Packet::id_ptr(buff);       // get id of packet
    const uint32_t* size = Packet::size_ptr(buff);    // get size of packet

    if(*id == AcnPacket::PACKET_ID)
    {
        std::cout << get_msg("INFO / CLIENT") << "Successfully generated path." << std::endl;
    }
    else if(*id == ErrorPacket::PACKET_ID)
    {
        if(mem != nullptr && (*mem)[PATH_SERVER].recv_packed_id == -1)
        {
            (*mem)[GENERAL].sync.lock();
            (*mem)[PATH_SERVER].errorpacket.allocate(*size);
            (*mem)[PATH_SERVER].errorpacket.set(buff);
            (*mem)[PATH_SERVER].errorpacket.decode();
            (*mem)[GENERAL].sync.unlock();
            (*mem)[PATH_SERVER].recv_packed_id = *id;
        }
    }
    else if(*id == GoalPacket::PACKET_ID)
    {
        if(mem != nullptr && (*mem)[PATH_SERVER].recv_packed_id == -1)
        {  
            (*mem)[GENERAL].sync.lock();
            (*mem)[PATH_SERVER].goalpacket.allocate(*size);
            (*mem)[PATH_SERVER].goalpacket.set(buff);
            (*mem)[PATH_SERVER].goalpacket.decode();
            (*mem)[GENERAL].sync.unlock();
            (*mem)[PATH_SERVER].recv_packed_id = *id;
        }
    }
}

void Client::run_pathserver(std::atomic_bool* running, const std::string* imgfolder)
{
    *running = true;
    char cmd[256];
    sprintf(cmd, "path_server.exe.lnk %s", imgfolder->c_str());  // Generate command to call the server.
    std::cout << get_msg("INFO / SERVER") << "Starting pathserver: " << cmd << std::endl;
    int ret = 0;
    if((ret = std::system(cmd)) < 0)    // Start server
    {
        std::cout << get_msg("ERROR / SERVER") << "Server closed with code: " << ret << " / " << std::hex << ret << std::endl;
        throw std::runtime_error("Cant start path server.");  // System error if server is unable to start.
    }
    *running = false;
}

void Client::start_pathserver(std::atomic_bool* running, const std::string* imgfolder)
{
    std::thread serverthread = std::thread(Client::run_pathserver, running, imgfolder);   

    serverthread.detach();
    while(!*running) { std::this_thread::yield(); }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void Client::shutdown_pathserver(std::atomic_bool* running, Client::SharedMemory& sharedsimumem)
{
    // Compile exit packet.
    ExitPacket exit;
    exit.allocate(exit.min_size());
    exit.encode();

    const std::string& remoteaddress = sharedsimumem.client->sock().getpeername().get_addr();
    uint16_t remoteport = sharedsimumem.client->sock().getpeername().get_port();

    // Send exit packet to pathserver.
    sharedsimumem.client->send(exit.rawdata(), exit.size(), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    sharedsimumem.client->close();
    // Wait until server is stopped.
    std::cout << "test" << std::endl;
    while (*running) { std::this_thread::yield(); }
    std::cout << "test2" << std::endl;
}   