/******************************************************************************************************************************************
* Title:        Path server / host
* Programtitle: path_server(.exe)
* Author:       Michael Reim
* Date:         10.12.2020
* Description:
*   The program sends goal information via sockets after a request from a client.
*   The generation of the goals is also a server-side operarion.
*
*   Command syntax:
*       path_server.exe <path to image folder> [<flags>]
*   Command flags:
*       command has no flags
*
*   Return values:
*       0 -> Success!
*       -1 -> Could not open or create log file.
*       -2 -> Something was wrong with the command. For more information see the error messages.
*       -3 -> Error occured while starting server.
*
*   Note: in the code are sometimes for-loops that look like this:
*       int i;
*       for(i=0; i < end; i++)
*   and some are like this:
*       for(int i=0; i < end; i++)
*   According to the second example, 'i' has to be pushed to the stack and popped from the stack
*   every iteration of the loop. The first example may seem unnecessary but since you ended up 
*   with a lot of loop iterations, like gothrough a 4k picture, this trick can save you some runtime.
*   In case of a radixsort algotithm, with 10 000 000 elements to sort, the algorithm ran about
*   10% faster.
*
*   In my case, everytime when I ended up with a lot of loop-cycles I chose the first method and
*   if I ended up with just a few loop-cycles I chose the second method.
*   You can also go everytime for the first method!
*
* @version release 1.1.1
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* If there are any bugs, contact me!
******************************************************************************************************************************************/

#include <cstdio>   // for in- and output
#include <msh.h>    // for socket handler
#include <atomic>   // for atomic variables
#include <vector>   // for the vector container
#include <map>      // for the map container
#include <dirent.h> // for directory operations
#include <direct.h> // for directory operations
#include "SchwarmPacket/packet.h"

#define MIN_ARGLENGTH 2 // minimum argument length of the command

/*
*   This struct's purpose is to share data between multiple threads.
*   More precisely the main and the socket threads.
*/

struct SharedVariables
{
    std::atomic_bool running{false};            // Is used to set the running sate of the main thread.
    std::atomic_bool generating_path{false};    // Is used to determine if the server is currently generating a path from an image file.
    std::atomic_int8_t packet_id{-1};           // Is used to be able to process the packet in the main thread.

    // Raw data that gets shared between threads.
    Schwarm::PathGeneratePacket pathgenpacket;
    Schwarm::GoalReqPacket goalreqpacket;

    FILE* logfile;
};

/*
*   Struct to store and have an easier access to X and Y values.
*       float x -> X value of the coordinate.
*       float y -> Y value of the coordinate.
*/

struct Goal
{
    float x, y;
};

/*
*   "on_connect" is a function that is called by the socker-handler.
*   This function gets called whenever a client connects to the server.
*   Parameters:
*       cppsock::socket* socket -> A pointer to the socket.
*       void** persistant -> A pointer to a pointer to any data (-struct).
*       error_t error -> An error value to interrogate if there was an error with the 
*                        server-side acception of the socket.
*/

void on_connect(cppsock::socket*, void**, error_t);

/*
*   "on_disconnect" is a function that is called by the socker-handler.
*   This function gets called whenever a client disconnects to the server.
*   Furthermore, the function will also be called if the connection gets cancelled
*   by the server (e.g. server is shutting down).
*       cppsock::socket* socket -> A pointer to the socket.
*       void** persistant -> A pointer to a pointer to any data (-struct).
*/

void on_disconnect(cppsock::socket*, void**);

/*
*   "on_reveive" is a function that is called by the socker-handler.
*   This function will be called if the socket receives a packet.
*   The packet can be any data.
*       cppsock::socket* socket -> A pointer to the socket.
*       void** persistant -> A pointer to a pointer to any data (-struct).
*/

void on_receive(cppsock::socket*, void**, SH::data_channel);

/*
*   This function decodes the received packets and 
*   forwards certain packets to the main thread.
*   Parameters:
*       cppsock::socket* socket -> A pointer to the socket.
*       uint8_t* data -> Received data from the packet.
*       void** persistant -> A pointer to a pointer to any data (-struct).
*/

void process_packet(cppsock::socket*, uint8_t*, void**);

/*
*   Sends via a socket an error packet to the client.
*   Parameters:
*       cppsock::socket* socket -> A pointer to the socket.
*       Schwarm::packet_error error -> Error code (enum).
*/

void send_error(cppsock::socket*, Schwarm::packet_error);

/*
*   Returns the local time in hh:mm:ss.
*   Returns only hours, minutes and seconds
*   Requires at least a string of 48 characters.
*/
void gettime(char*);

/*
*   Returns the local date in ISO 8601 string.
*   Returns only years, months and days
*   Requires at least a string of 48 characters.
*/
void getdate(char*);

/* PROGRAMMED FUNCTIONS */

void on_connect(cppsock::socket* socket, void** persistant, error_t error)
{
    // Check for 'nullptr' is not needed because there are not 2 different servers / clients used.
    SharedVariables* shared_variables = (SharedVariables*)*persistant;
    char time[48];

    /* Handler the errors that may occur when a connection gets accepted. */
    gettime(time);
    if(error & SH::error_code_t::HANDLER_NOT_RUNNING)       // This error occures if the socket-handler is not running.
    {
        fprintf(shared_variables->logfile, "[%s] [ERROR] Connection refused because socket-handler is not running.", time);
        fprintf(shared_variables->logfile, "[%s] [INFO] IP-Address / Hostname: %s:%hu\n", time, socket->getpeername().get_addr().c_str(), socket->getpeername().get_port());
    }
    else if(error & SH::error_code_t::SERVER_LIMIT)         // This error occures if the server has reached its limit of connections.
    {
        fprintf(shared_variables->logfile, "[%s] [ERROR] Connection refused because server has reached maximum of connections.", time);
        fprintf(shared_variables->logfile, "[%s] [INFO] IP-Address / Hostname: %s:%hu\n", time, socket->getpeername().get_addr().c_str(), socket->getpeername().get_port());
    }
    else if(error & SH::error_code_t::SERVER_ACCEPT_ERROR)  // This error occures if the server was not able to accept the socket / connection.
    {
        fprintf(shared_variables->logfile, "[%s] [ERROR] Could not accept connection.", time);
    }
    else    // If everything worked well.
    {
        fprintf(shared_variables->logfile, "[%s] [INFO] Client connected.\n", time);
        fprintf(shared_variables->logfile, "[%s] [INFO] IP-Address / Hostname: %s:%hu\n", time, socket->getpeername().get_addr().c_str(), socket->getpeername().get_port());
    }
}

void on_disconnect(cppsock::socket* socket, void** persistant)
{
    SharedVariables* shared_variables = (SharedVariables*)*persistant;
    char time[48];
    gettime(time);
    fprintf(shared_variables->logfile, "[%s] [INFO] Client disconnected.\n", time);
}

void on_receive(cppsock::socket* socket, void** persistant, SH::data_channel channel)
{
    uint8_t buff1[5];
    /*
    *   For the first time only read the first 5 bytes from the buffer and only peek it.
    *   Because of the peek flag, the those bytes will not be deleted from the buffer.
    *   This is done to get the size of the whole packet without modifying the receive buffer.
    */
    socket->recv(buff1, sizeof(buff1), channel | MSG_PEEK);
    const size_t* packet_size = Schwarm::Packet::size_ptr(buff1);   // Get the size of the packet.
    uint8_t buff2[*packet_size];                    // Create a second buffer with the size of the packet.
    /*
    *   Receive the second time now with the full size of the packet and without peeking so that the
    *   buffer gets cleared after reading the data from it.
    */
    socket->recv(buff2, sizeof(buff2), channel);    

    // Process the packet...
    process_packet(socket, buff2, persistant);
}

void process_packet(cppsock::socket* socket, uint8_t* data, void** persistant)
{
    const uint8_t* id = Schwarm::Packet::id_ptr(data);              // Get pointer to packet id.
    const size_t* size = Schwarm::Packet::size_ptr(data);           // Get pointer to size of packet.
    SharedVariables* shared_variables = (SharedVariables*)*persistant;    // Get pointer to shared memory.
    char time[48];

    gettime(time);
    if(*id == Schwarm::ExitPacket::PACKET_ID)
    {
        /*  If exit command was received set running value to 'false'.
        The server will shut down. */
        fprintf(shared_variables->logfile, "[%s] [INFO] Received stop command.\n", time);
        shared_variables->running = false;
    }
    else if(*id == Schwarm::PathGeneratePacket::PACKET_ID)
    {
        /*  Only process the packet if the main thread is still running (highest priority)
            and only if the server is not already generating a path. */
        if(shared_variables->running && !shared_variables->generating_path)
        {
            shared_variables->pathgenpacket.allocate(*size); // Allocate memory for the packet.
            shared_variables->pathgenpacket.set(data);       // Set the data string.
            shared_variables->pathgenpacket.decode();        // Decode the packet.

            fprintf(shared_variables->logfile, 
                    "[%s] [INFO] Generating goals for file %s with %u goals for vehicle %d...\n", 
                    time, 
                    shared_variables->pathgenpacket.get_filepath(), 
                    shared_variables->pathgenpacket.get_num_goals(), 
                    shared_variables->pathgenpacket.get_vehicle_id());
            // Set values for shared memory.
            shared_variables->generating_path = true;     // Set "generating_path" to 'true' to let the rest of the server know that a path is being generated.
            shared_variables->packet_id = shared_variables->pathgenpacket.id();
            /*  NOTE: It is important that setting the packed id is the last operation to ensure a synchronization
            *   betwenn this and the main thread. The main thread will only start to process the packet if the
            *   id has been set.
            */
        }
        else if(shared_variables->running && shared_variables->generating_path)
        {
            // If the server is busy generating goals, send an error to the client.
            fprintf(shared_variables->logfile, "[%s] [INFO] Server is already generating goals.\n", time);
            send_error(socket, Schwarm::packet_error::PACKET_SERVER_BUSY);
        }
    }
    else if(*id == Schwarm::GoalReqPacket::PACKET_ID)
    {
        fprintf(shared_variables->logfile, "[%s] [INFO] Received goal request.\n", time);
        if(shared_variables->running && !shared_variables->generating_path)
        {
            shared_variables->goalreqpacket.allocate(*size); // Allocate memory for the packet.
            shared_variables->goalreqpacket.set(data);       // Set the packet data.
            shared_variables->goalreqpacket.decode();        // Decode the packet.

            /*  The sending mechanic takes place in the main thread because 
            *   the vector for the goals is located threre.
            */

            // Set values for shared memory.
            shared_variables->packet_id = shared_variables->goalreqpacket.id();    // The same thing with the id like before...
        }
        else if(shared_variables->running && shared_variables->generating_path)
        {
            // If the server is busy generating goals, send an error to the client.
            fprintf(shared_variables->logfile, "[%s] [INFO] Can't send goal because server has not finished generating goals\n", time);
            send_error(socket, Schwarm::packet_error::PACKET_SERVER_BUSY);
        } 
    }
}

void send_error(cppsock::socket* socket, Schwarm::packet_error error)
{
    // For information that this function does see the prototype.
    Schwarm::ErrorPacket packet;
    packet.set_code(error);                             // Set the error code.
    packet.allocate(packet.min_size());                 // Allocate memory for the packet.
    packet.encode();                                    // Encode the packet.
    socket->send(packet.rawdata(), packet.size(), 0);   // Send it to the client.
}

void gettime(char* timestr)
{
    int64_t timenow;
    _time64(&timenow);
    tm* local_time = _localtime64(&timenow);

    // Convert time values to strings with a 0 at the begin if the number is smaller than 10.
    char hour_str[8], min_str[8], sec_str[8];
    sprintf(hour_str,   ((local_time->tm_hour < 10) ? "0%hd": "%hd"), (int16_t)local_time->tm_hour);
    sprintf(min_str,    ((local_time->tm_min < 10)  ? "0%hd": "%hd"), (int16_t)local_time->tm_min);
    sprintf(sec_str,    ((local_time->tm_sec < 10)  ? "0%hd": "%hd"), (int16_t)local_time->tm_sec);
    sprintf(timestr,    "%s:%s:%s", hour_str, min_str, sec_str);
}

void getdate(char* datestr)
{
    int64_t timenow;
    _time64(&timenow);
    tm* local_time = _localtime64(&timenow);

    // Convert time values to strings with a 0 at the begin if the number is smaller than 10.
    char mon_str[8], day_str[8];
    sprintf(mon_str,    ((local_time->tm_mon < 10)  ? "0%hd": "%hd"), (int16_t)local_time->tm_mon + 1);
    sprintf(day_str,    ((local_time->tm_mday < 10) ? "0%hd": "%hd"), (int16_t)local_time->tm_mday);
    sprintf(datestr,    "%d-%s-%s", local_time->tm_year + 1900, mon_str, day_str);
}

// its showtime
int main(int argc, const char* const * const argv)
{
    std::map<int, std::vector<Goal>> goals;    // The vector where the goals from the file are saved.

    SharedVariables shared_variables;   // The shared-data struct.
    shared_variables.running = true;  // Set running to true because the main thrad should be in the running state.
  
    /* CREATE DIRECTORIES */
    // Path to the directories.
    constexpr char LOG_DIRECTORY[] = "./logs";

    // Time % date string value
    char time[48], date[48];

    // Try to open the directories.
    DIR* log_dir = opendir(LOG_DIRECTORY);

    // Check if the directories exist and if not create them.
    if(log_dir == nullptr)
        mkdir(LOG_DIRECTORY);

    // Close the directories if they are open.
    closedir(log_dir);

    /* CREATE LOG FILE */
    char filepath[256];
    getdate(date);
    sprintf(filepath, "%s/%s_log.txt", LOG_DIRECTORY, date);
    shared_variables.logfile = fopen(filepath, "a");
    if(shared_variables.logfile == nullptr)
    {
        printf("[ERROR] Could not open or create log file: \"%s\"\n", filepath);
        printf("Exit code -1\n");
        return -1;
    }

    /* DECODE COMMAND */
    if(argc < MIN_ARGLENGTH)
    {
        // Number of arguments equals argc - 1 because the first element is the command (name of the executable) itself.
        printf("[ERROR] At least 1 argument requiered, given: %d\n", argc - 1);
        printf("Exit code -2\n");
        return -2;
    }
    else if(argc > MIN_ARGLENGTH)
    {
        // Number of arguments equals argc - 1 because the first element is the command (name of the executable) itself.
        printf("[ERROR] Too many arguments given: %d, requiered: 1\n", argc - 1);
        printf("Exit code -2\n");
        return -2;
    }
    fprintf(shared_variables.logfile, "\n--------------------------------------------------\n");

    /* START SOCKET HANDLER */
    SH::SocketHandler handler;
    handler.start();  
    gettime(time);              
    fprintf(shared_variables.logfile, "[%s] [INFO] Started socket-handler.\n", time);
    
    /* START SERVER */
    SH::Server server(handler, 1);              // The server class that accepts connections.
    *server.persist_ptr() = &shared_variables;    // Let the persistant pointer point to the shared-memory struct.
    server.set_callbacks(on_connect, on_disconnect, on_receive);    // Set the callbacks for this server.
    if(server.start("0.0.0.0", 10000, 1) != 0)                      // Start the server and interrogare for occured errors.
    {
        printf("[ERROR] Error occured while starting server.\n");
        printf("Exit code -3\n");
        return -3;
    }
    gettime(time);
    fprintf(shared_variables.logfile, "[%s] [INFO] Started server.\n", time);

    /* THE MAIN LOOP */
    while(shared_variables.running)
    {   
        // If a PathGeneratePacket gehts transmitted to the main thread.
        if(shared_variables.packet_id == Schwarm::PathGeneratePacket::PACKET_ID)
        {
            if(strcmp(shared_variables.pathgenpacket.get_filepath(), "%delete") == 0)
            {
                // If this command gets received delete the goals for this vehicle id.
                // Is usefull for a dynamic number of vehicles to delete unused memory.
                goals[shared_variables.pathgenpacket.get_vehicle_id()].clear();
                gettime(time);
                fprintf(shared_variables.logfile, "[%s] [INFO] Deleting goals for vehicle id %d.\n", time, shared_variables.pathgenpacket.get_vehicle_id());
            }
            else
            {
                // Build the command to call the "pathgenerator.exe" program.
                char cmd[256];
                sprintf(cmd, "pathgenerator.exe.lnk %s/%s goals.gol %u -nodebug -log %s", argv[1], shared_variables.pathgenpacket.get_filepath(), shared_variables.pathgenpacket.get_num_goals(), (shared_variables.pathgenpacket.should_invert()) ? "-invert" : "");
                //printf("%s\n", cmd);

                // Call the program "pathgenerator.exe".
                if(std::system(cmd) != 0)
                {
                    /*  If the std::system returns something unequal 0 (the return code of the program will be returned),
                    *   the generation of the goals has been failed.
                    *   For more information see the logfile of the "pathgenerator.exe" progarm.
                    */
                    gettime(time);
                    fprintf(shared_variables.logfile, "[%s] [ERROR] Failed to generate goals.\n", time);
                    // Send an error back to the cient to let it know that the generation has been failed.
                    send_error(&server.get_socket(0), Schwarm::packet_error::PACKET_FAILED_GENERATING_PATH);
                }
                else
                {
                    /*  If the path has successfully been generated, the goals can be
                    *   read from the file.
                    */
                    FILE* file = fopen("pathgenerator/goals.gol", "r");   // Open the file, the filename is defined.
                    if(file == NULL)
                    {
                        /*  If the file could not be opened the output file could not be opened
                        *   or the file has been deleted.
                        */
                        gettime(time);
                        fprintf(shared_variables.logfile, "[%s] [ERROR] Could not find path to goal-output file.\n", time);
                        send_error(&server.get_socket(0), Schwarm::packet_error::PACKET_FAILED_GENERATING_PATH);
                    }
                    else
                    {
                        /* If the file can be open, the reading process can begin. */
                        // Clear the goal vector to write new goals into it.
                        goals[shared_variables.pathgenpacket.get_vehicle_id()].clear();
                        Goal goal;
                        gettime(time);
                        fprintf(shared_variables.logfile, "[%s] [INFO] Reading goals...\n", time);
                        while(!feof(file))  // Read the whole file...
                        {
                            fscanf(file, "%f %f\n", &goal.x, &goal.y);          // Read one line and put the values into the "Goal" struct.
                            goals[shared_variables.pathgenpacket.get_vehicle_id()].push_back(goal); // Push the goal to the goal vector.
                        }
                        gettime(time);
                        fprintf(shared_variables.logfile, "[%s] [INFO] Successfully generated goals for vehicle %d.\n", time, shared_variables.pathgenpacket.get_vehicle_id());
                        fclose(file);   // Close the previously opened file.

                        // Send acnoledge.
                        Schwarm::AcnPacket packet;
                        packet.allocate(packet.min_size());
                        packet.encode();
                        server.get_socket(0).send(packet.rawdata(), packet.size(), 0);
                    }
                }
            }

            // After processing the packet, reset the shared memory.
            shared_variables.packet_id = -1;          // Set packet id to -1 because -1 indicates that no packet was received.
            shared_variables.generating_path = false; // Last but nor least, let the rest of the server know that the generation of the goals has been finished.

            /*
            *   NOTE: it is important that the "generating_path" set to 'false' is the last operation that should be done when processing
            *   this packet to enshure the syncronization between the receiver and the main thread.
            *   The receiver thread will only be accepting new packets if the value of "generating_path" is 'false', therefore
            *   this should be the last operation.
            */
        }
        // If a GoalReqPacket gets transmitted to the main thread.
        else if(shared_variables.packet_id == Schwarm::GoalReqPacket::PACKET_ID)
        {
            /*  If the request for the goal contains a too big number for the index (bigger than the size of the vector)
            *   an error will be sent to the client to let it know that the index was invalid.
            */
            if(shared_variables.goalreqpacket.get_goal_index() >= goals[shared_variables.goalreqpacket.get_vehicle_id()].size())
            {
                gettime(time);
                fprintf(shared_variables.logfile, "[%s] [ERROR] Received invalid goal index %u for vehicle %d (Maximum index: %u).\n", time, shared_variables.goalreqpacket.get_goal_index(), shared_variables.goalreqpacket.get_vehicle_id(), goals[shared_variables.goalreqpacket.get_vehicle_id()].size() - 1);
                // Send the actual packet.
                send_error(&server.get_socket(0), Schwarm::packet_error::PACKET_INVALID_GOAL);
            }
            else
            {
                // Otherwise, instead of an error, send the goal to the client if the index is valid.
                Schwarm::GoalPacket packet;
                packet.set_goal(goals[shared_variables.goalreqpacket.get_vehicle_id()].at(shared_variables.goalreqpacket.get_goal_index()).x, goals[shared_variables.goalreqpacket.get_vehicle_id()].at(shared_variables.goalreqpacket.get_goal_index()).y);  // Set the goal values.
                packet.set_vehicle_id(shared_variables.goalreqpacket.get_vehicle_id());
                packet.allocate(packet.min_size()); // Allocate memory for the packet.
                packet.encode();                    // Encode the packet to the byte string.
                server.get_socket(0).send(packet.rawdata(), packet.size(), 0);  // Sent the packet to the client.
                gettime(time);
                fprintf(shared_variables.logfile, "[%s] [INFO] Sent goal with index %u for vehicle %d.\n", time, shared_variables.goalreqpacket.get_goal_index(), shared_variables.goalreqpacket.get_goal_index());
            }
            // After processing the packet, reset the shared memory.
            shared_variables.packet_id = -1;  // Set packet id to -1 because -1 indicates that no packet was received.
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));  // for low CPU usage
    }
    server.stop();  // Stop the server.
    gettime(time);
    fprintf(shared_variables.logfile, "[%s] [INFO] Stopped server.\n", time);

    handler.stop(); // Stop the socket handler.
    gettime(time);
    fprintf(shared_variables.logfile, "[%s] [INFO] Stoppend socket-handler.\n", time);

    fprintf(shared_variables.logfile, "[%s] Exit code 0\n", time);
    return 0; // You have been terminated.
}