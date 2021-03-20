/*****************************************************************************************
* Title:        Shader-library
* Author:       Michael Reim
* Date:         29.07.2020
* Description:  Library for simple and easy vertex- and fragment-shader implementation for OpenGL.
* Features:     Loading (GLSL) shader-source-code from files.
*               Simple implementation.
*               Easy error management.
*
* @version release 1.0.1
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* If there are any bugs, contact me!
*****************************************************************************************/

#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <string>

namespace gl
{

    enum ShaderLoadErrorType : int
    {
        NONE = 0x0,
        SHADER_ALREADY_LOADED   = 0x1,
        INVALID_FILE_PATH       = 0x2,
        VERTEX_SHADER_ERROR     = 0x4,
        FRAGMENT_SHADER_ERROR   = 0x8,
        SHADER_LINK_ERROR       = 0x10
    };

    typedef int ShaderLoadError;

    class Shader
    {
    private:
        unsigned int shader_id;
        size_t shader_info_max_size;
        std::string vertex_shader_msg;
        std::string fragment_shader_msg;
        std::string shader_link_msg;

    public:
        Shader(void);
        Shader(size_t shader_info_max_size);
        Shader(const Shader& shader);
        Shader(Shader&& shader);
        virtual ~Shader(void);

        Shader& operator= (const Shader& shader);
        Shader& operator= (Shader&& shader);

        void set_info_max_size(size_t size) noexcept                                {this->shader_info_max_size = size;}
        unsigned int get_id(void) const noexcept                                    {return this->shader_id;}
        const std::string& get_last_vertex_msg(void) const noexcept                 {return this->vertex_shader_msg;}
        const std::string& get_last_fragment_msg(void) const noexcept               {return this->fragment_shader_msg;}
        const std::string& get_last_link_msg(void) const noexcept                   {return this->shader_link_msg;}
        size_t get_info_max_size(void) const noexcept                               {return this->shader_info_max_size;}
        int get_uniform_location(const std::string& uniform_name) const noexcept;

        ShaderLoadError load(const std::string& vertex_path, const std::string& fragment_path);
        void use(void) noexcept;
        void unuse(void) noexcept;
        void clear_msg(void) noexcept;
        void del(void) noexcept;

        bool uniform_1d(const std::string& uniform_name, double d1) noexcept;
        bool uniform_1dv(const std::string& uniform_name, size_t count, const double* dv) noexcept;
        bool uniform_1f(const std::string& uniform_name, float f1) noexcept;
        bool uniform_1fv(const std::string& uniform_name, size_t count, const float* fv) noexcept;
        bool uniform_1i(const std::string& uniform_name, int i1) noexcept;
        bool uniform_1iv(const std::string& uniform_name, size_t count, const int* iv) noexcept;
        bool uniform_1ui(const std::string& uniform_name, unsigned int ui1) noexcept;
        bool uniform_1uiv(const std::string& uniform_name, size_t count, const unsigned int* uiv) noexcept;

        bool uniform_2d(const std::string& uniform_name, double d1, double d2) noexcept;
        bool uniform_2dv(const std::string& uniform_name, size_t count, const double* dv) noexcept;
        bool uniform_2f(const std::string& uniform_name, float f1, float f2) noexcept;
        bool uniform_2fv(const std::string& uniform_name, size_t count, const float* fv) noexcept;
        bool uniform_2i(const std::string& uniform_name, int i1, int i2) noexcept;
        bool uniform_2iv(const std::string& uniform_name, size_t count, const int* iv) noexcept;
        bool uniform_2ui(const std::string& uniform_name, unsigned int ui1, unsigned int ui2) noexcept;
        bool uniform_2uiv(const std::string& uniform_name, size_t count, const unsigned int* uiv) noexcept;

        bool uniform_3d(const std::string& uniform_name, double d1, double d2, double d3) noexcept;
        bool uniform_3dv(const std::string& uniform_name, size_t count, const double* dv) noexcept;
        bool uniform_3f(const std::string& uniform_name, float f1, float f2, float f3) noexcept;
        bool uniform_3fv(const std::string& uniform_name, size_t count, const float* fv) noexcept;
        bool uniform_3i(const std::string& uniform_name, int i1, int i2, int i3) noexcept;
        bool uniform_3iv(const std::string& uniform_name, size_t count, const int* iv) noexcept;
        bool uniform_3ui(const std::string& uniform_name, unsigned int ui1, unsigned int ui2, unsigned int ui3) noexcept;
        bool uniform_3uiv(const std::string& uniform_name, size_t count, const unsigned int* uiv) noexcept;

        bool uniform_4d(const std::string& uniform_name, double d1, double d2, double d3, double d4) noexcept;
        bool uniform_4dv(const std::string& uniform_name, size_t count, const double* dv) noexcept;
        bool uniform_4f(const std::string& uniform_name, float f1, float f2, float f3, float f4) noexcept;
        bool uniform_4fv(const std::string& uniform_name, size_t count, const float* fv) noexcept;
        bool uniform_4i(const std::string& uniform_name, int i1, int i2, int i3, int i4) noexcept;
        bool uniform_4iv(const std::string& uniform_name, size_t count, const int* iv) noexcept;
        bool uniform_4ui(const std::string& uniform_name, unsigned int ui1, unsigned int ui2, unsigned int ui3, unsigned int ui4) noexcept;
        bool uniform_4uiv(const std::string& uniform_name, size_t count, const unsigned int* uiv) noexcept;

        bool uniform_matrix_2x2f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept;
        bool uniform_matrix_2x2d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept;
        bool uniform_matrix_2x3f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept;
        bool uniform_matrix_2x3d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept;
        bool uniform_matrix_2x4f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept;
        bool uniform_matrix_2x4d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept;

        bool uniform_matrix_3x2f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept;
        bool uniform_matrix_3x2d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept;
        bool uniform_matrix_3x3f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept;
        bool uniform_matrix_3x3d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept;
        bool uniform_matrix_3x4f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept;
        bool uniform_matrix_3x4d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept;

        bool uniform_matrix_4x2f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept;
        bool uniform_matrix_4x2d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept;
        bool uniform_matrix_4x3f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept;
        bool uniform_matrix_4x3d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept;
        bool uniform_matrix_4x4f(const std::string& uniform_name, size_t count, bool transponse, const float* ptr) noexcept;
        bool uniform_matrix_4x4d(const std::string& uniform_name, size_t count, bool transponse, const double* ptr) noexcept;
    };
};

#endif // SHADER_H_INCLUDED
