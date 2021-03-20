#include "../vehicle.h"

#include <iostream>

Schwarm::Vehicle::Vehicle(void)
{
    this->x_pos = 0.0f;
    this->y_pos = 0.0f;
    this->z_pos = 0.0f;

    this->x_angle = 0.0f;
    this->y_angle = 0.0f;
    this->z_angle = 0.0f;

    this->speed = 0.0f;
    this->needs_goal = false;
    this->next_goal_x = 0.0f;
    this->next_goal_y = 0.0f;
    this->dir = glm::vec2(0.0f, 0.0f);
}

Schwarm::Vehicle::Vehicle(const Vehicle& obj)
{
    *this = obj;
}

Schwarm::Vehicle& Schwarm::Vehicle::operator= (const Schwarm::Vehicle& obj)
{
    this->model_mat = obj.model_mat;
    return *this;
}

Schwarm::Vehicle::Vehicle(Vehicle&& obj)
{
    *this = std::move(obj);
}

Schwarm::Vehicle& Schwarm::Vehicle::operator= (Schwarm::Vehicle&& obj)
{
    this->model_mat = obj.model_mat;
    obj.model_mat = glm::mat4(0.0f);

    return *this;
}

Schwarm::Vehicle::~Vehicle(void)
{
    // dtor
}

void Schwarm::Vehicle::rotate(float x_angle, float y_angle, float z_angle) noexcept
{
    this->x_angle = x_angle;
    this->y_angle = y_angle;
    this->z_angle = z_angle;
}

void Schwarm::Vehicle::translate(float x, float y, float z) noexcept
{
    this->x_pos = x;
    this->y_pos = y;
    this->z_pos = z;
}

void Schwarm::Vehicle::set_next_goal(float x, float y) noexcept
{
    this->next_goal_x = x;
    this->next_goal_y = y;
}

void Schwarm::Vehicle::calc(void) noexcept
{

    this->model_mat = glm::mat4(1.0f);
    this->model_mat = glm::translate(this->model_mat, glm::vec3(to_float(this->x_pos), to_float(this->y_pos), to_float(this->z_pos)));
    
    this->model_mat = glm::rotate(this->model_mat, to_float(this->x_angle), glm::vec3(1.0f, 0.0f, 0.0f));
    this->model_mat = glm::rotate(this->model_mat, to_float(this->y_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    this->model_mat = glm::rotate(this->model_mat, to_float(this->z_angle), glm::vec3(0.0f, 0.0f, 1.0f));

}   