#ifndef __schwarm_vehicle_h__
#define __schwarm_vehicle_h__

#include <glm/glm.hpp>                  // for glm::mat4
#include <glm/gtc/matrix_transform.hpp> // for matrix transformations (translate, rotate)
#include <glm/gtc/type_ptr.hpp>         // for type pointer for matrices
#include <CL/opencl.h>                  // for OpenCL video-RAM-memory
#include <Model.h>                      // for custom models for the vehicles
#include <vector>                       // for vector container
#include <atomic>                       // for atomic variables
#include <exception>                    // for exception handling
#include "../client/client.h"           // for shared simulation memory

namespace Schwarm
{
    class shared_memory_null : public std::exception
    {  
    public:
        virtual const char* what(void) const throw()
        {
            return "Shared memory of vehicle-processor is a nullptr.";
        }
    };

    class Vehicle
    {
        using atomic_float = std::atomic<float>;
    private:
        glm::mat4 model_mat;

        atomic_float x_angle, y_angle, z_angle;         // rotation of the vehicle
        atomic_float x_pos, y_pos, z_pos;               // position of the vehicle

        atomic_float speed;                             // speed of the vehicle
        atomic_float next_goal_x, next_goal_y;          // next goal target
        atomic_float targetdistance;                    // distance to the next target
        glm::vec2 dir;                                  // move direction
        atomic_float dis;                               // view direction of the next target goal
        atomic_float old_targetangle, new_targetangle;  // View angle at the target, "new_targetangle" -> at the "next target goal"
        std::atomic_bool needs_goal;                    // indicator if vehicle needs a goal

        inline static float to_float(const atomic_float& af)
        {
            const float f = af; // Copy value of float to ensure the value is allocated in the stack of the current thread.
                                // Casting would be bad since the value is not in the same thread, in such a case
                                // casting is going to destroy the atomicness.
            return f;
        }
    
    public:
        Vehicle(void);

        Vehicle(const Vehicle& obj);
        Vehicle& operator= (const Vehicle& obj);
        
        Vehicle(Vehicle&& obj);
        Vehicle& operator= (Vehicle&& obj);

        virtual ~Vehicle(void);

        void rotate(float, float, float) noexcept;
        void translate(float, float, float) noexcept;
        void calc(void) noexcept;

        float pos_x(void) const noexcept                        {return this->x_pos;}
        float pos_y(void) const noexcept                        {return this->y_pos;}
        float pos_z(void) const noexcept                        {return this->z_pos;}

        float angle_x(void) const noexcept                      {return this->x_angle;}
        float angle_y(void) const noexcept                      {return this->y_angle;}
        float angle_z(void) const noexcept                      {return this->z_angle;}

        void  set_speed(float speed) noexcept                   {this->speed = speed;}
        float get_speed(void) const noexcept                    {return this->speed;}

        void  set_next_goal(float, float) noexcept;
        float get_next_goal_x(void) const noexcept              {return this->next_goal_x;}
        float get_next_goal_y(void) const noexcept              {return this->next_goal_y;}
    
        void set_new_targetangle(float f) noexcept              {this->new_targetangle = f;}
        void set_old_targetangle(float f) noexcept              {this->old_targetangle = f;}
        float get_new_targetangle(void) const noexcept          {return this->new_targetangle;}
        float get_old_targetangle(void) const noexcept          {return this->old_targetangle;}

        void set_goal_needed(bool b) noexcept                   {this->needs_goal = b;}
        bool goal_needed(void) const noexcept                   {return this->needs_goal;}

        glm::vec2& direction(void) noexcept                     {return this->dir;}
        const glm::vec2& direction(void) const noexcept         {return this->dir;}

        void set_distance(float f) noexcept                     {this->dis = f;}
        float get_distance(void) const noexcept                 {return this->dis;}

        const glm::mat4& get_model_mat(void) const noexcept     {return this->model_mat;}

    };

    class VehicleBuffer
    {
    private:
        unsigned int vao_vehicle;
        unsigned int vbo_vehicle;
        unsigned int vbo_vehicle_mat;
        cl_mem cl_vbo_vehicle_mat;

        cl_context* context;
        cl_command_queue* cmd_queue;

        std::vector<Vehicle*> vehicles;
        bool dynamic_vehicles;

        size_t max_vehicles;

    public:
        VehicleBuffer(void)                             = delete;
        VehicleBuffer(cl_context& context, cl_command_queue& cmd_queue, gl::Model& model, bool dynamic_vehicles = false, size_t max_vehicles = 1);

        VehicleBuffer(const VehicleBuffer&)             = delete;
        VehicleBuffer& operator= (const VehicleBuffer&) = delete;

        VehicleBuffer(VehicleBuffer&&)                  = delete;
        VehicleBuffer& operator= (VehicleBuffer&&)      = delete;

        virtual ~VehicleBuffer(void);

        void add_vehicle(Vehicle*);
        void remove_vehicle(size_t);
        void update_vehicle(size_t);

        Vehicle* get_vehicle(size_t idx)                {return this->vehicles.at(idx);}
        const Vehicle* get_vehicle(size_t idx) const    {return this->vehicles.at(idx);}
        size_t get_max_vehicles(void) const noexcept    {return this->max_vehicles;}
        size_t get_num_vehicles(void) const noexcept    {return this->vehicles.size();}

        unsigned int get_vao(void) const noexcept       {return this->vao_vehicle;}
    };

    class VehicleProcessor
    {
        using atomic_float = std::atomic<float>;
    private:
        VehicleBuffer* vehicle_buffer;
        atomic_float tablesize_x, tablesize_y;
        atomic_float tableorigin_x, tableorigin_y;

        thread processor_thread;
        std::atomic_bool running;
#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
        std::mutex processor_mutex;
#else
        mingw_stdthread::xp::mutex processor_mutex;
#endif
        Schwarm::Client::SharedSimulationMemory* sharedsimumem;
        std::chrono::milliseconds tickspeed;

        static void process(VehicleProcessor*);

        /*
        *   Checks if the simulation is still running.
        *   Parameters:
        *       const bool* states -> Bool array to the simulation states of the vehicles.
        *       size_t size -> Size of the bool array.
        *   @return 'true' if a vehicle is processed, 'false' if no vehicle is processed anymore.
        */
        static bool simu_finished(const bool*, size_t);

        /**
        *   Function for a linear interpolation.
        *   @param a -> Base value.
        *   @param b -> Value a gehts interpolated to.
        *   @param w -> weight (weight = 0.0, erg = a; weight = 1.0, erg = b)
        */
        static float lerp(float a, float b, float w)
        {
            return (b * w) + (a * (1 - w));
        }

        static float absf(float x)
        {
            return (x < 0) ? x * -1.0f : x;
        }

    public:
        VehicleProcessor(Schwarm::Client::SharedSimulationMemory* mem = nullptr);

        VehicleProcessor(const VehicleProcessor&) = delete;
        VehicleProcessor& operator=(const VehicleProcessor&) = delete;

        VehicleProcessor(VehicleProcessor&&) = delete;
        VehicleProcessor& operator=(VehicleProcessor&&) = delete;

        virtual ~VehicleProcessor(void);

        /**
        *   Sets the vehicle-buffer that should be processed.
        *   @param VehicleBuffer* -> The buffer that should be processed.
        *   @return 'true' if buffer was able to set, 'false' if buffer couldn't be set.
        */
        bool set_buffer(VehicleBuffer*) noexcept;

        /*
        *   Sets the size of the table.
        *   Is used to be able to move the vehicles properly.
        *   Parameters:
        *       float size_x -> X-size of the table.
        *       float size_y -> Y-size if the table.
        */
        void set_tablesize(float, float) noexcept;

        /*
        *   Sets the origin of the table.
        *   Is used to be able to move the vehicles properly.
        *   Parameters:
        *       float x -> X-coordinate of the origin.
        *       float y -> Y-coordinate of the origin.
        */
        void set_tableorigin(float, float) noexcept;

        /*
        *   Sets the internal tickspeed in milliseconds.
        *   Parameters:
        *       std::chrono::milliseconds ms -> Tick-speed in milleseconds.
        */
        void set_tickspeed(std::chrono::milliseconds) noexcept;

        /*
        *   Starts the vehicle-processor.
        */
        void start(void);

        /*
        *   Stopps the vehicle-processor and deletes dynamic memory if dynamic vehicles is set.
        */
        void stop(void);
    };
};

#endif //__schwarm_vehicle_h__