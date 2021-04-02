#include "../vehicle.h"
#include <iostream>
#include <cmath>
#include "../../includes/my_msg.h"

#ifndef M_PI
    #define M_PI    3.14159265358979323846
#endif

using Schwarm::Vehicle;
using Schwarm::VehicleProcessor;

/* CONSTRUCTORS */

VehicleProcessor::VehicleProcessor(std::map<Schwarm::Client::ClientType, Schwarm::Client::SharedMemory>* mem)
{
    this->running = false;
    this->shared_memory = mem;
    this->set_tablesize(0.0f, 0.0f);
    this->set_tableorigin(0.0f, 0.0f);
    this->tickspeed = std::chrono::milliseconds(0);
}

/* DESTRUCTOR */

VehicleProcessor::~VehicleProcessor(void)
{
    this->stop();
}

/* PRIVATE METHODS */

void VehicleProcessor::process(VehicleProcessor* processor)
{
    while(processor->running)
    { 
        VehicleBuffer* vehicle_buffer = processor->vehicle_buffer;

        // If simulation should start.
        if((*processor->shared_memory)[Schwarm::Client::GENERAL].start)
        {
            std::cout << get_msg("INFO / SIMU") << "Started simulation." << std::endl;
            std::cout << get_msg("INFO / SIMU") << "Simulation is processed for " << vehicle_buffer->get_num_vehicles() / 2 << " vehicles." << std::endl;

            (*processor->shared_memory)[Schwarm::Client::GENERAL].sync.lock();
            bool real = (*processor->shared_memory)[Schwarm::Client::GENERAL].real;
            (*processor->shared_memory)[Schwarm::Client::GENERAL].sync.unlock();

            bool* simu_states = new bool[vehicle_buffer->get_num_vehicles() / 2];
            // Initializes the all states to true.
            for(size_t i=0; i < vehicle_buffer->get_num_vehicles(); i=+2)
            {
                simu_states[i] = true;
                vehicle_buffer->get_vehicle(i)->set_goal_needed(true);
            }

            uint32_t* goal_indices = new uint32_t[vehicle_buffer->get_num_vehicles() / 2];
            for(size_t i=0; i < vehicle_buffer->get_num_vehicles(); i+=2)
                goal_indices[i] = 0;

            std::chrono::time_point tstart = std::chrono::high_resolution_clock::now();
            do
            {
                uint64_t deltatime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - tstart).count();
                tstart = std::chrono::high_resolution_clock::now();
                for (size_t i = 0; i < vehicle_buffer->get_num_vehicles(); i+=2)
                {
                    Vehicle* cur_vehicle = vehicle_buffer->get_vehicle(i);
                    Vehicle* cur_vehicle_real = vehicle_buffer->get_vehicle(i + 1);
                    // Get goal if vehicles hasn't got one.
                    if (simu_states[i/2] && cur_vehicle->goal_needed())
                    {
                        // Create request for goal.
                        GoalReqPacket request;
                        request.set_goal_index(goal_indices[i / 2]++);
                        request.set_vehicle_id(i / 2);
                        request.allocate(request.min_size());
                        request.encode();
                        // Send request for a goal.
                        (*processor->shared_memory)[Schwarm::Client::PATH_SERVER].client->send(request.rawdata(), request.size(), 0);

                        // Wait until a packet is received (either a GoalPacket or an ErrorPacket).
                        // break the waiting if processor was stopped or simulation was stopped
                        while (processor->running &&
                            (*processor->shared_memory)[Schwarm::Client::GENERAL].start &&
                            (*processor->shared_memory)[Schwarm::Client::PATH_SERVER].recv_packed_id == -1)
                        {
                            std::this_thread::yield();
                        }

                        // Error case
                        if ((*processor->shared_memory)[Schwarm::Client::PATH_SERVER].recv_packed_id == ErrorPacket::PACKET_ID)
                        {
                            // Simulation is finished if client receives a PACKET_INVALID_GOAL
                            // Mutex requiered because .get_code() is not atomic.
                            (*processor->shared_memory)[Schwarm::Client::GENERAL].sync.lock();
                            bool is_invalid = ((*processor->shared_memory)[Schwarm::Client::PATH_SERVER].errorpacket.get_code() == Schwarm::packet_error::PACKET_INVALID_GOAL);
                            (*processor->shared_memory)[Schwarm::Client::GENERAL].sync.unlock();
                            if (is_invalid)
                                std::cout << get_msg("INFO / SIMU") << "Simulation finished for vehicle " << i/2 << std::endl;
                            else
                                std::cout << get_msg("ERROR / SIMU") << "Simultion failed for vehicle " << i/2 << std::endl;
                            simu_states[i/2] = false;
                        }
                        else if ((*processor->shared_memory)[Schwarm::Client::PATH_SERVER].recv_packed_id == GoalPacket::PACKET_ID)
                        {
                            // Get the received normalized texture coordinates.
                            (*processor->shared_memory)[Schwarm::Client::GENERAL].sync.lock();
                            const float ntc_x = (*processor->shared_memory)[Schwarm::Client::PATH_SERVER].goalpacket.get_goal_x();
                            const float ntc_y = (*processor->shared_memory)[Schwarm::Client::PATH_SERVER].goalpacket.get_goal_y();
                            (*processor->shared_memory)[Schwarm::Client::GENERAL].sync.unlock();

                            // Calculate the real position from the normalized coordinates.
                            // Formula: origin + size * normalized_position
                            const float pos_x = processor->tableorigin_x + processor->tablesize_x * ntc_x;
                            const float pos_y = processor->tableorigin_y + processor->tablesize_y * ntc_y;

                            // Calculate move direction vector
                            glm::vec2 oldpos(cur_vehicle->pos_x(), cur_vehicle->pos_z());
                            glm::vec2 newpos(pos_x, pos_y);
                            cur_vehicle->set_distance(glm::distance(oldpos, newpos));   // distance curpos - new pos
                            newpos = glm::normalize(newpos - oldpos);   // reuse "newpos" as new direction


                            // set direction vector
                            (*processor->shared_memory)[Schwarm::Client::GENERAL].sync.lock();
                            glm::vec2 olddir = cur_vehicle->direction();    // get old direction
                            cur_vehicle->direction() = newpos;              // update old direction to new direction
                            (*processor->shared_memory)[Schwarm::Client::GENERAL].sync.unlock();

                            // set target of vehicle
                            cur_vehicle->set_next_goal(pos_x, pos_y);
                            cur_vehicle->set_goal_needed(false);

                            // calculate angle of new direction vector in (-180 to +180)
                            glm::vec2 baseline_vector(1.0f, 0.0f);  // vector of the base line
                            float alpha = acosf(glm::dot(olddir, baseline_vector)) * ((olddir.y > 0) ? -1.0f : 1.0f);
                            float beta = acosf(glm::dot(newpos, baseline_vector)) * ((newpos.y > 0) ? -1.0f : 1.0f);

                            // only do the check if alpha and beta have a different sign
                            if ((alpha < 0 && beta >= 0))
                            {
                                // convert alpha from (-180 to +180) to (0 to 360)
                                const float alpha360 = 2 * M_PI + alpha;
                                // compare if alpha or alpha360 is the shortest angle to beta, and set alphs to the shortest
                                alpha = (VehicleProcessor::absf(beta - alpha360) < VehicleProcessor::absf(beta - alpha)) ? alpha360 : alpha;
                            }
                            else if (alpha >= 0 && beta < 0)
                            {
                                // convert beta from (-180 to +180) to (0 to 360)
                                const float beta360 = 2 * M_PI + beta;
                                // compare if alpha is the shortest angle to beta or beta360, and set beta to the shortest
                                beta = (VehicleProcessor::absf(beta360 - alpha) < VehicleProcessor::absf(beta - alpha)) ? beta360 : beta;
                            }
                            cur_vehicle->set_old_targetangle(alpha);
                            cur_vehicle->set_new_targetangle(beta);
                        }
                        (*processor->shared_memory)[Schwarm::Client::PATH_SERVER].recv_packed_id = -1;
                    }
                    // Move vehicle if vehicle has goal and simulation is processed is for that vehicle
                    if (simu_states[i / 2] && !cur_vehicle->goal_needed())
                    {
                        float detec_pos_x;
                        float detec_pos_y;

                        // only calculate difference if visualization is in real-life mode
                        if (real)
                        {
                            const float detec_ntc_x = (*processor->shared_memory)[Schwarm::Client::DETECTION_SERVER].detec_coords[(i / 2)].x;
                            const float detec_ntc_y = (*processor->shared_memory)[Schwarm::Client::DETECTION_SERVER].detec_coords[(i / 2)].y;

                            detec_pos_x = processor->tableorigin_x + processor->tablesize_x * detec_ntc_x;
                            detec_pos_y = processor->tableorigin_y + processor->tablesize_y * detec_ntc_y;

                            cur_vehicle_real->translate(detec_pos_x, 0.015f, detec_pos_y);
                        }

                        // calculate move distance
                        // s = v * t
                        float delta_s = cur_vehicle->get_speed() * (deltatime / 1000000000.0);  // use deltatime in seconds
                        glm::vec2 delta_pos = cur_vehicle->direction() * delta_s;               // dpos = direction * ds

                        // get distance to goal
                        // Distance_vec = Goal - Position
                        // Distance = sqrt(Distance_vec.x ^ 2 + Distance_vec.y ^ 2)
                        float remain_dis = glm::distance(glm::vec2(cur_vehicle->get_next_goal_x(), cur_vehicle->get_next_goal_y()),
                            glm::vec2(cur_vehicle->pos_x(), cur_vehicle->pos_z()));

                        // check if translation would go over the goal
                        // if not ... translate
                        if (remain_dis > delta_s)
                        {
                            // move vehicle along the direction vector
                            cur_vehicle->translate(delta_pos.x + cur_vehicle->pos_x(), 0.015f, delta_pos.y + cur_vehicle->pos_z());
                            const float new_angle = lerp(cur_vehicle->get_new_targetangle(), cur_vehicle->get_old_targetangle(), remain_dis / cur_vehicle->get_distance());
                            cur_vehicle->rotate(0.0f, new_angle, 0.0f);
                        }
                        else // if so set it to goal's position
                        {
                            cur_vehicle->translate(cur_vehicle->get_next_goal_x(), 0.015f, cur_vehicle->get_next_goal_y());
                            cur_vehicle->rotate(0.0f, cur_vehicle->get_new_targetangle(), 0.0f);
                            cur_vehicle->set_goal_needed(true);
                        }
                        cur_vehicle->calc();
                        vehicle_buffer->update_vehicle(i);

                        if (real)
                        {
                            cur_vehicle_real->calc();
                            vehicle_buffer->update_vehicle(i + 1);
                        }
                    }
                }
            }
            while(processor->running && (*processor->shared_memory)[Schwarm::Client::GENERAL].start && !simu_finished(simu_states, vehicle_buffer->get_num_vehicles() / 2));
            /*  1) Interrupt simulation if thread (vehicle-processor) shuts down.
            *   2) Interrupt simulation if the start boolean is 'false' (user interrupt).
            *   3) Interrupt simulation if simulation has finished.
            */

            if(processor->running && (*processor->shared_memory)[Schwarm::Client::GENERAL].start)
                std::cout << get_msg("INFO / SIMU") << "Simulation finished." << std::endl;
            else
                std::cout << get_msg("INFO / SIMU") << "Simulation interrupted." << std::endl;
            delete[](simu_states);
            delete[](goal_indices);
            (*processor->shared_memory)[Schwarm::Client::GENERAL].start = false;
        }
        // While waiting for simulation to start, sleep 5 milliseconds for low CPU-usage.
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

bool VehicleProcessor::simu_finished(const bool* states, size_t size)
{
    for(size_t i=0; i < size; i++)
    {
        if(states[i])
            return false;
    }
    return true;
}

/* PUBLIC METHODS */

bool VehicleProcessor::set_buffer(VehicleBuffer* buffer) noexcept
{   
    // Return fals if simulation is running.
    if((*this->shared_memory)[Schwarm::Client::GENERAL].start)
        return false;
    // Mutex used for milti-threading synchronization.
    this->processor_mutex.lock();
    this->vehicle_buffer = buffer;
    this->processor_mutex.unlock();
    return true;
}

void VehicleProcessor::set_tablesize(float x, float y) noexcept
{
    this->tablesize_x = x;
    this->tablesize_y = y;
}

void VehicleProcessor::set_tableorigin(float x, float y) noexcept
{
    this->tableorigin_x = x;
    this->tableorigin_y = y;
}

void VehicleProcessor::set_tickspeed(std::chrono::milliseconds time) noexcept
{
    this->processor_mutex.lock();
    this->tickspeed = time;
    this->processor_mutex.unlock();
}

void VehicleProcessor::start(void)
{
    if(!this->running)
    {
        if(this->shared_memory == nullptr)
            throw Schwarm::shared_memory_null();

        this->processor_thread = std::thread(process, this);
        this->running = true;
    }
}

void VehicleProcessor::stop(void)
{
    if(this->running)
    {
        this->running = false;
        this->processor_thread.join();
    }
}