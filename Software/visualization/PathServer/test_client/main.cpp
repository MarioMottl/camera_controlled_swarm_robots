#include <msh.h>
#include <cstdio>
#include <conio.h>
#include <iostream>
#include <cstring>
#include "../SchwarmPacket/packet.h"

using char_128_ptr_t = char(*)[128];    // pointer to char[128]

void on_connect(cppsock::socket*, void**, error_t);
void on_disconnect(cppsock::socket*, void**);
void on_receive(cppsock::socket*, void**, SH::data_channel);

int decode_command(char* buff, char(*ret_args)[128]);
void on_command(int len, char(*args)[128], SH::Client& client);
void process_packet(uint8_t* buff);

void on_connect(cppsock::socket* socket, void** persistant, error_t error)
{
    if(error != SH::error_code_t::HANDLER_NO_ERROR)
    {
        printf("Error code: %d\n", error);
        exit(-1);
    }
    printf("%s:%hu\n", socket->getpeername().get_addr().c_str(), socket->getpeername().get_port());
}

void on_disconnect(cppsock::socket* socket, void** persistant)
{
    printf("Disconnected.\n");
}

void on_receive(cppsock::socket* socket, void** persistant, SH::data_channel channel)
{
    uint8_t buff1[5];
    socket->recv(buff1, sizeof(buff1), channel | MSG_PEEK);
    const size_t* packet_size = Schwarm::Packet::size_ptr(buff1);
    uint8_t buff2[*packet_size];
    socket->recv(buff2, sizeof(buff2), channel); 

    process_packet(buff2);
}

int decode_command(char* buff, char(*ret_args)[128])
{
    char* ptr = strtok(buff, " \t");
    int i = 0;
    while(ptr != nullptr)
    {
        strcpy(ret_args[i++], ptr);
        ptr = strtok(nullptr, " \t");
    }
    return i;
}

void on_command(int len, char(*args)[128], SH::Client& client)
{
    if(strcmp(args[0], "generate") == 0)
    {
        // generate <path> <num goals> <vehicle id>
        if(len != 4)
            return;
        unsigned int num_goals;
        int vehicle_id;
        sscanf(args[2], "%u", &num_goals);
        sscanf(args[3], "%d", &vehicle_id);

        Schwarm::PathGeneratePacket packet;
        packet.set_filepath(args[1]);
        packet.set_num_goals(num_goals);
        packet.set_vehicle_id(vehicle_id);
        packet.should_invert() = false;
        packet.allocate(packet.min_size() + packet.filepath_size());
        packet.encode();
        client.get_socket().send(packet.rawdata(), packet.size(), 0);
        printf("Sent generate packet\n");
    }
    else if(strcmp(args[0], "goal") == 0)
    {
        // goal <index> <vehicle id>
        if(len != 3)
            return;
        size_t idx;
        int vehicle_id;
        sscanf(args[1], "%u", &idx);
        sscanf(args[2], "%d", &vehicle_id);

        Schwarm::GoalReqPacket packet;
        packet.set_goal_index(idx);
        packet.set_vehicle_id(vehicle_id);
        packet.allocate(packet.min_size());
        packet.encode();
        client.get_socket().send(packet.rawdata(), packet.size(), 0);
        printf("Sent goal request packet\n");
    }
    else if(strcmp(args[0], "exit") == 0)
    {
        // exit
        if(len != 1)
            return;

        Schwarm::ExitPacket packet;
        packet.allocate(packet.min_size());
        packet.encode();
        client.get_socket().send(packet.rawdata(), packet.size(), 0);
        printf("Sent exit packet\n");
    }
}

void process_packet(uint8_t* buff)
{
    const uint8_t* id = Schwarm::Packet::id_ptr(buff);
    const size_t* size = Schwarm::Packet::size_ptr(buff);

    if(*id == Schwarm::AcnPacket::PACKET_ID)
    {
        printf("Successfully generated path.");
    }
    else if(*id == Schwarm::GoalPacket::PACKET_ID)
    {
        Schwarm::GoalPacket packet;
        packet.allocate(*size);
        packet.set(buff);
        packet.decode();

        printf("GOAL -> X: %f Y: %f Vehicle: %d\n", packet.get_goal_x(), packet.get_goal_y(), packet.get_vehicle_id());
    }
    else if(*id == Schwarm::ErrorPacket::PACKET_ID)
    {
        Schwarm::ErrorPacket packet;
        packet.allocate(*size);
        packet.set(buff);
        packet.decode();

        printf("%s", Schwarm::Packet::strerror(packet.get_code()));
    }
}

int main()
{
    SH::SocketHandler handler;
    handler.start();

    SH::Client client(handler);
    client.set_callbacks(on_connect, on_disconnect, on_receive);
    client.connect("localhost", 10000);
    printf("Connected to server.\n");

    while(!(kbhit() && getch() == 27))
    {
        std::string textbuff;
        std::getline(std::cin, textbuff);

        char args[16][128];
        int arglen = decode_command(&textbuff[0], args);

        on_command(arglen, args, client);
    }

    client.disconnect();
    handler.stop();
    return 0;
}