#include "../Shader.h"
#include <GL/glew.h>

using namespace gl;

Shader::Shader(void) : Shader(1024) {}

Shader::Shader(size_t shader_info_max_size)
{
    this->shader_id = 0;
    this->shader_info_max_size = shader_info_max_size;
}

Shader::Shader(const Shader& shader)
{
    *this = shader;
}

Shader::Shader(Shader&& shader)
{
    *this = std::move(shader);
}

Shader::~Shader(void)
{
    this->clear_msg();
    this->del();
}

Shader& Shader::operator= (const Shader& shader)
{
    this->shader_id = shader.shader_id;
    this->shader_info_max_size = shader.shader_info_max_size;
    this->vertex_shader_msg = shader.vertex_shader_msg;
    this->fragment_shader_msg = shader.fragment_shader_msg;
    this->shader_link_msg = shader.shader_link_msg;
    return *this;
}

Shader& Shader::operator= (Shader&& shader)
{
    this->shader_id = std::move(shader.shader_id);
    this->shader_info_max_size = std::move(shader.shader_info_max_size);
    this->vertex_shader_msg = std::move(shader.vertex_shader_msg);
    this->fragment_shader_msg = std::move(shader.fragment_shader_msg);
    this->shader_link_msg = std::move(shader.shader_link_msg);
    return *this;
}

int Shader::get_uniform_location(const std::string& uniform_name) const noexcept
{
    return glGetUniformLocation(this->shader_id, uniform_name.c_str());
}

void Shader::use(void) noexcept
{
    if(this->shader_id > 0)
        glUseProgram(this->shader_id);
}

void Shader::unuse(void) noexcept
{
    if(this->shader_id > 0)
        glUseProgram(0);
}

void Shader::clear_msg(void) noexcept
{
    this->vertex_shader_msg.clear();
    this->fragment_shader_msg.clear();
    this->shader_link_msg.clear();
}

void Shader::del(void) noexcept
{
    glDeleteProgram(this->shader_id);
    this->shader_id = 0;
}