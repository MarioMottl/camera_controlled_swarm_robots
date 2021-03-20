#include "../Shader.h"
#include <GL/glew.h>

using namespace gl;

/* ----------------------------------------------- 1D Uniforms ----------------------------------------------- */
bool Shader::uniform_1d(const std::string& uniform_name, double d1) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform1d(loc, d1);
    return true;
}

bool Shader::uniform_1dv(const std::string& uniform_name, size_t count, const double* dv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform1dv(loc, count, dv);
    return true;
}

bool Shader::uniform_1f(const std::string& uniform_name, float f1) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform1f(loc, f1);
    return true;
}

bool Shader::uniform_1fv(const std::string& uniform_name, size_t count, const float* fv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform1fv(loc, count, fv);
    return true;
}

bool Shader::uniform_1i(const std::string& uniform_name, int i1) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform1i(loc, i1);
    return true;
}

bool Shader::uniform_1iv(const std::string& uniform_name, size_t count, const int* iv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform1iv(loc, count, iv);
    return true;
}

bool Shader::uniform_1ui(const std::string& uniform_name, unsigned int ui1) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform1ui(loc, ui1);
    return true;
}

bool Shader::uniform_1uiv(const std::string& uniform_name, size_t count, const unsigned int* uiv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform1uiv(loc, count, uiv);
    return true;
}

/* ----------------------------------------------- 2D Uniforms ----------------------------------------------- */

bool Shader::uniform_2d(const std::string& uniform_name, double d1, double d2) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform2d(loc, d1, d2);
    return true;
}

bool Shader::uniform_2dv(const std::string& uniform_name, size_t count, const double* dv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform2dv(loc, count, dv);
    return true;
}

bool Shader::uniform_2f(const std::string& uniform_name, float f1, float f2) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform2f(loc, f1, f2);
    return true;
}

bool Shader::uniform_2fv(const std::string& uniform_name, size_t count, const float* fv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform2fv(loc, count, fv);
    return true;
}

bool Shader::uniform_2i(const std::string& uniform_name, int i1, int i2) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform2i(loc, i1, i2);
    return true;
}

bool Shader::uniform_2iv(const std::string& uniform_name, size_t count, const int* iv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform2iv(loc, count, iv);
    return true;
}

bool Shader::uniform_2ui(const std::string& uniform_name, unsigned int ui1, unsigned int ui2) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform2ui(loc, ui1, ui2);
    return true;
}

bool Shader::uniform_2uiv(const std::string& uniform_name, size_t count, const unsigned int* uiv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform2uiv(loc, count, uiv);
    return true;
}

/* ----------------------------------------------- 3D Uniforms ----------------------------------------------- */

bool Shader::uniform_3d(const std::string& uniform_name, double d1, double d2, double d3) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform3d(loc, d1, d2, d3);
    return true;
}

bool Shader::uniform_3dv(const std::string& uniform_name, size_t count, const double* dv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform3dv(loc, count, dv);
    return true;
}

bool Shader::uniform_3f(const std::string& uniform_name, float f1, float f2, float f3) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform3f(loc, f1, f2, f3);
    return true;
}

bool Shader::uniform_3fv(const std::string& uniform_name, size_t count, const float* fv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform3fv(loc, count, fv);
    return true;
}

bool Shader::uniform_3i(const std::string& uniform_name, int i1, int i2, int i3) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform3i(loc, i1, i2, i3);
    return true;
}

bool Shader::uniform_3iv(const std::string& uniform_name, size_t count, const int* iv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform3iv(loc, count, iv);
    return true;
}

bool Shader::uniform_3ui(const std::string& uniform_name, unsigned int ui1, unsigned int ui2, unsigned int ui3) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform3ui(loc, ui1, ui2, ui3);
    return true;
}

bool Shader::uniform_3uiv(const std::string& uniform_name, size_t count, const unsigned int* uiv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform3uiv(loc, count, uiv);
    return true;
}

/* ----------------------------------------------- 4D Uniforms ----------------------------------------------- */

bool Shader::uniform_4d(const std::string& uniform_name, double d1, double d2, double d3, double d4) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform4d(loc, d1, d2, d3, d4);
    return true;
}

bool Shader::uniform_4dv(const std::string& uniform_name, size_t count, const double* dv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform4dv(loc, count, dv);
    return true;
}

bool Shader::uniform_4f(const std::string& uniform_name, float f1, float f2, float f3, float f4) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform4f(loc, f1, f2, f3, f4);
    return true;
}

bool Shader::uniform_4fv(const std::string& uniform_name, size_t count, const float* fv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform4fv(loc, count, fv);
    return true;
}

bool Shader::uniform_4i(const std::string& uniform_name, int i1, int i2, int i3, int i4) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform4i(loc, i1, i2, i3, i4);
    return true;
}

bool Shader::uniform_4iv(const std::string& uniform_name, size_t count, const int* iv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform4iv(loc, count, iv);
    return true;
}

bool Shader::uniform_4ui(const std::string& uniform_name, unsigned int ui1, unsigned int ui2, unsigned int ui3, unsigned int ui4) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform4ui(loc, ui1, ui2, ui3, ui4);
    return true;
}

bool Shader::uniform_4uiv(const std::string& uniform_name, size_t count, const unsigned int* uiv) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniform4uiv(loc, count, uiv);
    return true;
}

/* ----------------------------------------------- Matrix 2 Uniforms ----------------------------------------------- */
bool Shader::uniform_matrix_2x2f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix2fv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_2x2d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix2dv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_2x3f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix2x3fv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_2x3d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix2x3dv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_2x4f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix2x4fv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_2x4d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix2x4dv(loc, count, transponse, ptr);
    return true;
}

/* ----------------------------------------------- Matrix 3 Uniforms ----------------------------------------------- */
bool Shader::uniform_matrix_3x2f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix3x2fv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_3x2d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix3x2dv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_3x3f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix3fv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_3x3d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix3dv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_3x4f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix3x4fv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_3x4d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix3x4dv(loc, count, transponse, ptr);
    return true;
}

/* ----------------------------------------------- Matrix 4 Uniforms ----------------------------------------------- */
bool Shader::uniform_matrix_4x2f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix4x2fv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_4x2d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix4x2dv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_4x3f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix4x3fv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_4x3d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix4x3dv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_4x4f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix4fv(loc, count, transponse, ptr);
    return true;
}

bool Shader::uniform_matrix_4x4d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept
{
    unsigned int loc = this->get_uniform_location(uniform_name);
    if(loc < 0)
        return false;
    glUniformMatrix4dv(loc, count, transponse, ptr);
    return true;
}