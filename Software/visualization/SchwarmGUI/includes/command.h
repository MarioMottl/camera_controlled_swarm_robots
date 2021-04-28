#ifndef __command_h__
#define __command_h__

#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include "my_utility.h"
#include "../Vehicle/vehicle.h"

namespace Schwarm
{
    /*
    *   Splits the command into arguments.
    *   Parameter:
    *       std::string str -> String to split.
    *       std::vector<std::string>& args -> The returned arguments.
    */
    static bool decode_command(std::string str, std::vector<std::string>& args)
    {
        if(str.size() > 0 && str[0] == '/')
        {
            char* cmdstr = str.data() + 1;          // ignore the '/' at the begin
            const char delim[] = " \t";

            char* token = strtok(cmdstr, delim);    // split string into tokens
            while(token != nullptr)                 // iterate through every token
            {
                args.push_back(token);
                token = strtok(nullptr, delim);
            }

            return true;
        }
        return false;
    }

    /*
    *   This function is callen if a command gets executed.
    *   Parameter:
    *       const std::vector<std::string>& args -> Readonly arguments of the command.
    */

    static void on_command(const std::vector<std::string>& args, std::map<Schwarm::Client::ClientType, Schwarm::Client::SharedMemory>* shared_memory)
    {   
        constexpr char simu_syntax[] = "Syntax: \n/simu\n/simu start|stop\n/simu generate <image-file> <vehicle id> <number of goals>\n/simu reset <vehicle id>";
        // Simu command
        // Syntax: /simu -> Connects to simulation server.
        // Syntax: /simu generate <image-file> <vehicle id> <number of goals> -> Generates a path from image file.
        if(args.size() > 0 && args[0] == "simu")
        {
            if(args.size() > 1)
            {
                if(args[1] == "generate")
                {
                    if(args.size() != 5)
                    {
                        std::cout << get_msg("ERROR / CLIENT") << simu_syntax << std::endl;
                        return;
                    }
                    if(!Schwarm::simu_connected((*shared_memory)[Schwarm::Client::PATH_SERVER].client)) // Only send packet if the client is connected to the simulation server.
                    {
                        std::cout << get_msg("ERROR / CLIENT") << "Path-Server is not running, start Path-Server and restart visualization." << std::endl;
                        return;
                    }
                    if(!Schwarm::is_number(args[3]))    // Check if <vehicle id> is a number.
                    {
                        std::cout << get_msg("ERROR / CLIENT") << "Argument <vehicle id> \"" << args[3] << "\" is not a number." << std::endl;
                        return;
                    }
                    if(!Schwarm::is_number(args[4]))    // Check if <number of goals> is a number.
                    {
                        std::cout << get_msg("ERROR / CLIENT") << "Argument <number of goals> \"" << args[4] << "\" is not a number." << std::endl;
                        return;
                    }   
                    int vehicle_id;
                    uint32_t number_of_goals;
                    sscanf(args[3].c_str(), "%d", &vehicle_id);         // Fetch <vehicle id> from argument.
                    sscanf(args[4].c_str(), "%u", &number_of_goals);    // Fetch <number of goals> from argument.

                    Schwarm::PathGeneratePacket packet;
                    packet.set_filepath(args[2].c_str());   // Initialize packet.
                    packet.set_vehicle_id(vehicle_id);
                    packet.set_num_goals(number_of_goals);
                    packet.allocate(packet.min_size() + packet.filepath_size());    // Allocate memory for packet.
                    packet.encode();    // Encode packet.

                    std::cout << get_msg("INFO / CLIENT") << "Generating path from file \"" << args[2] << "\"..." << std::endl;
                    (*shared_memory)[Schwarm::Client::PATH_SERVER].client->send(packet.rawdata(), packet.size(), 0);   // Send packet.
                }
                else if(args[1] == "start")
                {
                    if(args.size() != 2)
                    {
                        std::cout << get_msg("ERROR / CLIENT") << simu_syntax << std::endl;
                        return;
                    }
                    if(!Schwarm::simu_connected((*shared_memory)[Schwarm::Client::PATH_SERVER].client)) // Only send packet if the client is connected to the simulation server.
                    {
                        std::cout << get_msg("ERROR / CLIENT") << "Path-Server is not running, start Path-Server and restart visualization." << std::endl;
                        return;
                    }
                    if((*shared_memory)[Schwarm::Client::GENERAL].start)
                        std::cout << get_msg("ERROR / SIMU") << "Simulation is already running." << std::endl;
                    else
                        (*shared_memory)[Schwarm::Client::GENERAL].start = true;

                    // swarm control needs packet to initialize
                    if ((*shared_memory)[Schwarm::Client::GENERAL].real)
                    {
                        Schwarm::VehicleCommandPacket command;
                        command.allocate(command.min_size());
                        command.set_vehicle_id(0);
                        command.set_angle(0.0f);
                        command.set_length(0.0f);
                        command.encode();
                        (*shared_memory)[Schwarm::Client::CONTROL_SERVER].client->send(command.rawdata(), command.size(), 0);
                    }
                }
                else if(args[1] == "stop")
                {
                    if(args.size() != 2)
                    {
                        std::cout << get_msg("ERROR / CLIENT") << simu_syntax << std::endl;
                        return;
                    }
                    if(!(*shared_memory)[Schwarm::Client::GENERAL].start)
                        std::cout << get_msg("ERROR / SIMU") << "Simulation is not running." << std::endl;
                    else
                        (*shared_memory)[Schwarm::Client::GENERAL].start = false;
                }
                else if(args[1] == "reset")
                {
                    if(!Schwarm::simu_connected((*shared_memory)[Schwarm::Client::PATH_SERVER].client)) // Only send packet if the client is connected to the simulation server.
                    {
                        std::cout << get_msg("ERROR / CLIENT") << "Path-Server is not running, start Path-Server and restart visualization." << std::endl;
                        return;
                    }
                    if(args.size() != 3)
                    {
                        std::cout << get_msg("ERROR / CLIENT") << simu_syntax << std::endl;
                        return;
                    }
                    if(args.size() == 3)
                    {
                        uint32_t idx;
                        sscanf(args[2].c_str(), "%d", &idx);    // get index from string
                        // generate packet
                        Schwarm::PathGeneratePacket packet;
                        packet.set_filepath("%delete");         // %delete tells the server to delete the path
                        packet.set_vehicle_id(idx);     
                        packet.allocate(packet.min_size() + packet.filepath_size());
                        packet.encode();
                        (*shared_memory)[Schwarm::Client::PATH_SERVER].client->send(packet.rawdata(), packet.size(), 0);
                        std::cout << get_msg("INFO / CLIENT") << "Path resetted for vehicle: " << idx << "." << std::endl;
                    }
                }
                else
                {
                    std::cout << get_msg("ERROR / CLIENT") << "Invalid argument \"" << args[1] << "\"." << std::endl;
                }
            }
            else
                std::cout << get_msg("ERROR / CLIENT") << simu_syntax << std::endl;                     // too few arguments
        }
        else if (args.size() > 0 && args[0] == "real")
        {
            if ((*shared_memory)[Schwarm::Client::GENERAL].start)
            {
                std::cout << "Can't toggle visualization mode while visualization process is running." << std::endl;
                return;
            }

            Schwarm::VehicleBuffer* vehicles = (Schwarm::VehicleBuffer*)(*shared_memory)[Schwarm::Client::GENERAL].vehicles;

            // toggle real-live and simulation
            (*shared_memory)[Schwarm::Client::GENERAL].sync.lock();
            bool* b = (bool*)&(*shared_memory)[Schwarm::Client::GENERAL].real;
            *b = !*b;
            (*shared_memory)[Schwarm::Client::GENERAL].sync.unlock();

            // if real-life mode is active, enable secondary vehicles and set simulation (primary) vehicles transparent
            if (*b)
            {
                for (size_t i = 0; i < vehicles->get_num_vehicles(); i++)
                {
                    // primary vehicle
                    if (i % 2 == 0)
                    {
                        vehicles->get_vehicle(i)->set_opacity(0.5f);
                        vehicles->update_vehicle(i);
                    }
                    else // secondary vehicle
                    {
                        vehicles->get_vehicle(i)->translate(0.0f, 0.015f, 0.0f);
                        vehicles->get_vehicle(i)->calc();
                        vehicles->update_vehicle(i);
                    }
                }
            }
            else // disable secondary vehicles and set simulation (primary) vehicles to non-transparent
            {
                for (size_t i = 0; i < vehicles->get_num_vehicles(); i++)
                {
                    // primary vehicle
                    if (i % 2 == 0)
                    {
                        vehicles->get_vehicle(i)->set_opacity(-1.0f);
                        vehicles->update_vehicle(i);
                    }
                    else // secondary vehicle
                    {
                        vehicles->get_vehicle(i)->translate(0.0f, -1000000.0f, 0.0f);
                        vehicles->get_vehicle(i)->calc();
                        vehicles->update_vehicle(i);
                    }
                }
            }

            std::cout << ((*b) ? "Visualization is now in real-life mode!" : "Visualization is now in simulation mode") << std::endl;
        }
        else if(args.size() > 0)
            std::cout << get_msg("ERROR / CLIENT") << "Unknown command: /" << args[0] << std::endl;
    }
}

#endif // __command_h__