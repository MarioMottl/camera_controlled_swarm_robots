#include "../vehicle.h"
#include <GL/glew.h>
#include <GL/glfw.h>

#include <iostream>

Schwarm::VehicleBuffer::VehicleBuffer(cl_context& context, cl_command_queue& cmd_queue, gl::Model& model, bool dynamic_vehicles, size_t max_vehicles)
{
    static const size_t MODEL_STRIDE = sizeof(float) * (gl::Model::vertex_size() + gl::Model::texture_coord_size() + gl::Model::normal_size());
    
    this->context = &context;
    this->cmd_queue = &cmd_queue;

    this->dynamic_vehicles = dynamic_vehicles;
    this->max_vehicles = max_vehicles;

    glGenVertexArrays(1, &this->vao_vehicle);
    glGenBuffers(1, &this->vbo_vehicle);
    glGenBuffers(1, &this->vbo_vehicle_mat);

    glBindVertexArray(this->vao_vehicle);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vehicle);
    glBufferData(GL_ARRAY_BUFFER, model.vertex_data().size() * sizeof(float), model.vertex_data().data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, gl::Model::vertex_size(),          GL_FLOAT, false, MODEL_STRIDE, (void*)(gl::Model::vertex_offset()));
    glVertexAttribPointer(1, gl::Model::texture_coord_size(),   GL_FLOAT, false, MODEL_STRIDE, (void*)(gl::Model::texture_coord_offset()));
    glVertexAttribPointer(2, gl::Model::normal_size(),          GL_FLOAT, false, MODEL_STRIDE, (void*)(gl::Model::normal_offset()));

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vehicle_mat);
    glBufferData(GL_ARRAY_BUFFER, max_vehicles * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(3);   // 3 + 0
    glEnableVertexAttribArray(4);   // 3 + 1
    glEnableVertexAttribArray(5);   // 3 + 2
    glEnableVertexAttribArray(6);   // 3 + 3
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(0 * sizeof(glm::vec4)));
    glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
    glVertexAttribPointer(5, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glVertexAttribPointer(6, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->cl_vbo_vehicle_mat = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, this->vbo_vehicle_mat, NULL);
}

Schwarm::VehicleBuffer::~VehicleBuffer(void)
{
    glDeleteBuffers(1, &this->vbo_vehicle);
    glDeleteBuffers(1, &this->vbo_vehicle_mat);
    glDeleteVertexArrays(1, &vao_vehicle);

    if(this->dynamic_vehicles)
    {
        for(size_t i=0; i < this->vehicles.size(); i++)
            delete(this->vehicles.at(i));
    }
}

void Schwarm::VehicleBuffer::add_vehicle(Schwarm::Vehicle* vehicle)
{
    if(this->vehicles.size() < this->max_vehicles)
    {
        this->vehicles.push_back(vehicle);
        this->update_vehicle(this->vehicles.size() - 1);
    }
}

void Schwarm::VehicleBuffer::remove_vehicle(size_t idx)
{
    if(idx < this->vehicles.size())
    {
        this->vehicles.erase(this->vehicles.begin() + idx);
        for(size_t i = idx; i < this->vehicles.size(); i++)
            this->update_vehicle(i);
    }
}

void Schwarm::VehicleBuffer::update_vehicle(size_t idx)
{
    if(idx < this->vehicles.size())
    {
        const Vehicle* vehicle = this->vehicles.at(idx);
        clEnqueueAcquireGLObjects(*this->cmd_queue, 1, &this->cl_vbo_vehicle_mat, 0, NULL, NULL);
        clEnqueueWriteBuffer(*this->cmd_queue, this->cl_vbo_vehicle_mat, true, idx * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(vehicle->get_model_mat()), 0, NULL, NULL);
        clEnqueueReleaseGLObjects(*this->cmd_queue, 1, &this->cl_vbo_vehicle_mat, 0, NULL, NULL);
    }
}