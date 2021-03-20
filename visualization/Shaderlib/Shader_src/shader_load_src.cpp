#include "../Shader.h"
#include <cstdio>
#include <GL/glew.h>

using namespace gl;

inline static void fix_code(std::string& str)
{
    while(str.at(str.size() - 1) != '}')
        str.pop_back();
}

ShaderLoadError Shader::load(const std::string& vertex_path, const std::string& fragment_path)
{
    if(this->shader_id > 0)
        return ShaderLoadErrorType::SHADER_ALREADY_LOADED;

    // create shader src files
    FILE* vertex_file = fopen(vertex_path.c_str(), "r");
    FILE* fragment_file = fopen(fragment_path.c_str(), "r");

    // if path is invalid return error INVALID_FILE_PATH
    if(!vertex_file || !fragment_file)
        return ShaderLoadErrorType::INVALID_FILE_PATH;

    std::string vertex_code;
    std::string fragment_code;

    // read files
    while(!feof(vertex_file))
        vertex_code.push_back(fgetc(vertex_file));
    fix_code(vertex_code);
    fclose(vertex_file);

    while(!feof(fragment_file))
        fragment_code.push_back(fgetc(fragment_file));
    fix_code(fragment_code);
    fclose(fragment_file);

    const char* const vertex_code_ptr = vertex_code.c_str();       // help pointer to vertex_string
    const char* const fragment_code_ptr = fragment_code.c_str();   // help pointer to fragment_string

    //create info log buffers and init them with ASCII 0
    char vertex_info_log[this->shader_info_max_size]{'\0'};
    char fragment_info_log[this->shader_info_max_size]{'\0'};
    char link_info_log[this->shader_info_max_size]{'\0'};

    // create shaders
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // set vertex shader and get info log for vertex shader
    glShaderSource(vertex_shader, 1, &vertex_code_ptr, NULL);
    glCompileShader(vertex_shader);
    glGetShaderInfoLog(vertex_shader, this->shader_info_max_size, NULL, vertex_info_log);
    this->vertex_shader_msg = vertex_info_log;

    // set fragment shader and get info log for vertex shader
    glShaderSource(fragment_shader, 1, &fragment_code_ptr, NULL);
    glCompileShader(fragment_shader);
    glGetShaderInfoLog(fragment_shader, this->shader_info_max_size, NULL, fragment_info_log);
    this->fragment_shader_msg = fragment_info_log;

    // create shader program, init shader porgram and get info log for shader program
    this->shader_id = glCreateProgram();
    glAttachShader(this->shader_id, vertex_shader);
    glAttachShader(this->shader_id, fragment_shader);
    glLinkProgram(this->shader_id);
    glGetProgramInfoLog(this->shader_id, this->shader_info_max_size, NULL, link_info_log);
    this->shader_link_msg = link_info_log;

    // delete vertex and fragment shader
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // set errors if occured
    ShaderLoadError error = ShaderLoadErrorType::NONE;
    if(this->vertex_shader_msg.size() > 0)
        error |= ShaderLoadErrorType::VERTEX_SHADER_ERROR;
    if(this->fragment_shader_msg.size() > 0)
        error |= ShaderLoadErrorType::FRAGMENT_SHADER_ERROR;
    if(this->shader_link_msg.size() > 0)
        error |= ShaderLoadErrorType::SHADER_LINK_ERROR;

    return error;
}
