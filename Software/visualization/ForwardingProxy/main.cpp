#include <stdio.h>
#include <string.h>
#include <vector>
#include <conio.h>
#include <msh.h>
#include "lib/SchwarmPacket/packet.h"

//Declaration of the connection details that have to be send
//to the server
struct connection_details
{
    std::string name;
};

bool operator<(const connection_details &lhs, const connection_details &rhs)
{
    if(lhs.name < rhs.name) return true;
    else if (lhs.name > rhs.name) return false;
    return false;
}
//Map for waiting socket connection that have not been matched
using waiting_map = std::map<connection_details, cppsock::socket*>;

//Checkup if the necessary connections have been established
//TO-DO: Add Pruggmayer if necessary
bool allregistered(const waiting_map& wmap)
{
    static const std::vector<connection_details> details = {
        {"Mario"}, {"Michi"}
    };
    
    for(size_t i=0; i<details.size(); i++)
    {
        if(wmap.count(details.at(i)) < 1)
            return false;
    }
    return true;
}

bool is_equal(waiting_map& wmap, const std::string& name, const cppsock::socket* socket)
{
    return (wmap[{name}] == socket);
}

void delsocket(waiting_map& wmap, cppsock::socket* socket)
{
    for(auto iter = wmap.begin(); iter != wmap.end(); iter++)
    {
        if(iter->second == socket)
        {
            wmap.erase(iter);
            printf("deleted %s\n", iter->first.name.c_str());
            return;
        }
    }
}

void on_recv(cppsock::socket* socket, void** persistent, SH::data_channel channel)
{
    waiting_map* wmap = (waiting_map*)*persistent;  
    if(!allregistered(*wmap))
    {
        char identifier[16];
        socket->recv(identifier, sizeof(char[16]), 0);
        (*wmap)[{identifier}] = socket;
        if(allregistered(*wmap))
        {
            (*wmap)[{"Mario"}]->send("Ready\0", 6, 0);
            printf("verified!\n");
        }   
    }
    else
    {
        if(is_equal(*wmap, "Mario", socket))
        {
            uint8_t buff1[5];
            socket->recv(buff1, sizeof(buff1), MSG_PEEK);
            const size_t* packet_size = Schwarm::Packet::size_ptr(buff1);

            uint8_t buff2[*packet_size];
            socket->recv(buff2, sizeof(buff2), 0);

            Schwarm::GoalPacket recv_packet;
            recv_packet.allocate(*packet_size);
            recv_packet.set(buff2);
            recv_packet.decode();

            float x = recv_packet.get_goal_x();
            float y = recv_packet.get_goal_y();

            uint32_t* xip = (uint32_t*)&x;
            uint32_t* yip = (uint32_t*)&y;

            uint32_t revxi = ntohl(*xip);
            uint32_t revyi = ntohl(*yip);

            float* revx = (float*)&revxi;
            float* revy = (float*)&revyi;


            Schwarm::GoalPacket send_packet;
            send_packet.allocate(*packet_size);
            send_packet.set_goal(*revx, *revy);
            send_packet.set_vehicle_id(recv_packet.get_vehicle_id());
            send_packet.encode();

            printf("X:%f  Y:%f\n", send_packet.get_goal_x(), send_packet.get_goal_y());

            
            (*wmap)[{"Michi"}]->send(send_packet.rawdata(), send_packet.size(), 0);
        }
    }
}

void on_connect(cppsock::socket* socket, void** persistent, error_t error)
{
    printf("Client connected to the server. \n");
}

void on_disconnect(cppsock::socket* socket, void** persistent)
{
    printf("Client disconnected from server.\n");
    waiting_map* wmap = (waiting_map*)*persistent;
    delsocket(*wmap, socket);
}

int main()
{
    waiting_map wmap;

    SH::SocketHandler sh;
    sh.start();
    printf("Socket handler started.\n");

    SH::Server s(sh, 2);
    s.set_callbacks(on_connect, on_disconnect, on_recv);
    *s.persistent() = &wmap;
    s.start("0.0.0.0", 10005, 1);
    printf("Server started.\n");

    while(!(kbhit() && getch() == 27))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    s.stop();   
    sh.stop();
    return 0;
}