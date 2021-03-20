#ifndef __command_h__
#define __command_h__

#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include "my_utility.h"

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

    static void on_command(const std::vector<std::string>& args, Schwarm::Client::SharedSimulationMemory* sharedsimumem)
    {   
        constexpr char simu_syntax[] = "Syntax: \n/simu\n/simu start|stop\n/simu generate <image-file> <vehicle id> <number of goals>\n/simu reset <vehicle id>";
        // Simu command
        // Syntax: /simu -> Connects to simulation server.
        // Syntax: /simu generate <image-file> <vehicle id> <number of goals> -> Generates a path from image file.
        if(args[0] == "simu")
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
                    if(!Schwarm::simu_connected(sharedsimumem->client)) // Only send packet if the client is connected to the simulation server.
                    {
                        std::cout << get_msg("ERROR / CLIENT") << "You have to be in simulation mode to generate a path. (use: /simu)" << std::endl;
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

                    std::cout << get_msg("ERROR / CLIENT") << "Generating path from file \"" << args[2] << "\"..." << std::endl;
                    sharedsimumem->client->get_socket().send(packet.rawdata(), packet.size(), 0);   // Send packet.
                }
                else if(args[1] == "start")
                {
                    if(args.size() != 2)
                    {
                        std::cout << get_msg("ERROR / CLIENT") << simu_syntax << std::endl;
                        return;
                    }
                    if(!Schwarm::simu_connected(sharedsimumem->client)) // Only send packet if the client is connected to the simulation server.
                    {
                        std::cout << get_msg("ERROR / CLIENT") << "You have to be in simulation mode to start simulation. (use: /simu)" << std::endl;
                        return;
                    }
                    if(sharedsimumem->start)
                        std::cout << get_msg("ERROR / SIMU") << "Simulation is already running." << std::endl;
                    else
                        sharedsimumem->start = true;
                }
                else if(args[1] == "stop")
                {
                    if(args.size() != 2)
                    {
                        std::cout << get_msg("ERROR / CLIENT") << simu_syntax << std::endl;
                        return;
                    }
                    if(!sharedsimumem->start)
                        std::cout << get_msg("ERROR / SIMU") << "Simulation is not running." << std::endl;
                    else
                        sharedsimumem->start = false;
                }
                else if(args[1] == "reset")
                {
                    if(!Schwarm::simu_connected(sharedsimumem->client)) // Only send packet if the client is connected to the simulation server.
                    {
                        std::cout << get_msg("ERROR / CLIENT") << "You have to be in simulation mode to to reset simulation. (use: /simu)" << std::endl;
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
                        sharedsimumem->client->get_socket().send(packet.rawdata(), packet.size(), 0);
                        std::cout << get_msg("INFO / CLIENT") << "Path resetted for vehicle: " << idx << "." << std::endl;
                    }
                }
                else
                {
                    std::cout << get_msg("ERROR / CLIENT") << "Invalid argument \"" << args[1] << "\"." << std::endl;
                }
            }
        }
    }
}

#endif // __command_h__