#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/Shader.h>
#include <GL/glc.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <map>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glstr.h"

#define WINSX (1920 / 2)
#define WINSY (1080 / 2)
#define WPOSX (1920 / 4)
#define WPOSY (1080 / 4)
#define WIN_ASPECT (float)WINSX / (float)WINSY

using gl::Shader;
using gl::ShaderLoadError;
using gl::ShaderLoadErrorType;
using gl::Font;

struct StringData
{
    unsigned int vao{0}, vbo{0};
    uint16_t pos_x{0}, pos_y{0};
    glm::mat4 mvp = glm::mat4(1.0f);
};

void initWindow(int, int, int, int);
void create_text_shader(Shader&);
void gen_str_vao(const gl::string& str, StringData& data);

void initWindow(int xsize, int ysize, int xpos, int ypos)
{
    if(!glfwOpenWindow(xsize, ysize, 24, 24, 24, 24, 24, 0, GLFW_WINDOW))
    {
        glfwTerminate();
        exit(-1);
    }
    glfwSetWindowPos(xpos, ypos);
}

void create_text_shader(Shader& shader)
{
    ShaderLoadError error = shader.load("Shader/text_vertex_shader.txt", "Shader/text_fragment_shader.txt");

    if(error & ShaderLoadErrorType::INVALID_FILE_PATH)
        std::cout << "Can not open shader-source-file." << std::endl;
    if(error & ShaderLoadErrorType::SHADER_ALREADY_LOADED)
        std::cout << "Shader must be deleted before loading again." << std::endl;
    if(error & ShaderLoadErrorType::VERTEX_SHADER_ERROR)
        std::cout << "Vertex-Shader info: " << std::endl << shader.get_last_vertex_msg() << std::endl;
    if(error & ShaderLoadErrorType::FRAGMENT_SHADER_ERROR)
        std::cout << "Fragment-Shader info: " << std::endl << shader.get_last_fragment_msg() << std::endl;
    if(error & ShaderLoadErrorType::SHADER_LINK_ERROR)
        std::cout << "Shader-Link info: " << std::endl << shader.get_last_link_msg() << std::endl;

    if(error)
        exit(-1);
}

void gen_str_vao(const gl::string& str, StringData& data)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, str.buff_size(), str.mesh(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, str.mesh_stride(), (void*)gl::string::get_vertex_offset_ptr());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, str.mesh_stride(), (void*)gl::string::get_color_offset_ptr());
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, str.mesh_stride(), (void*)gl::string::get_tex_offset_ptr());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    data.vbo = vbo;
    data.vao = vao;
}

int main()
{
    int                 width, height;
    bool                running = true;
    const std::string   mainwindowname = "console";
    const std::string   consolename = "GLstring";

    glfwInit();
    initWindow(WINSX, WINSY, WPOSX, WPOSY);

    glfwSetWindowTitle(mainwindowname.c_str());
    SetConsoleTitleA(consolename.c_str());
    glewInit();
    glfwSwapInterval(1);

    Shader text_shader;
    create_text_shader(text_shader);

    Font console_font;
    gl::FontError error = console_font.load("console_font.png");
    if(error != gl::FontError::font_error_none)
        return -2;

    std::map<gl::string*, StringData*> str2D_resizeable;

    // init string 1
    gl::string str1("Hello World", console_font, gl::convert::from_pixels_size(50, WINSY));
    StringData str1_data;
    str1_data.pos_x = 0;
    str1_data.pos_y = 0;

    std::vector<float> colors = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    // just some testing
    str1.set_colors(colors);
    str1.push_back('X', 1.0f, 1.0f, 0.0f, 1.0f);
    str1.push_front('X', 1.0f, 1.0f, 0.0f, 0.4f);
    str1.insert(2, 'Y', 0.0f, 0.0f, 0.0f, 1.0f);
    str1.fill_color(0, 3, 0.0f, 0.0f, 0.0f, 0.5f);
    str1.erase_color(1);

    // init string 2
    gl::string str2("AB\tC", console_font, gl::convert::from_pixels_size(50, WINSY));
    StringData str2_data;
    str2_data.pos_x = 0;
    str2_data.pos_y = 50;
    str2.set_tab_size(0.5);

    // generate mesh of strings
    str1.generate_mesh();
    str2.generate_mesh();

    // init model-matrix of strings
    str1.set_pos(gl::convert::from_pixels_pos_x(str1_data.pos_x, WINSX) * WIN_ASPECT, gl::convert::from_pixels_pos_y(str1_data.pos_y, WINSY), 0.0f);
    str2.set_pos(gl::convert::from_pixels_pos_x(str2_data.pos_x, WINSX) * WIN_ASPECT, gl::convert::from_pixels_pos_y(str2_data.pos_y, WINSY), 0.0f);
    str1.calc_model("t");
    str2.calc_model("t");

    // generate buffer objects for gpu
    gen_str_vao(str1, str1_data);
    gen_str_vao(str2, str2_data);

    // register strings in map
    str2D_resizeable[&str1] = &str1_data;
    str2D_resizeable[&str2] = &str2_data;

    // init matrices
    glm::mat4 projection2D = glm::ortho(-WIN_ASPECT, WIN_ASPECT, -1.0f, 1.0f, -1.0f, 1.0f);
    glm::mat4 resize2D(1.0f);

    // set mvp matrices
    str1_data.mvp = projection2D * str1.model() * resize2D;
    str2_data.mvp = projection2D * str2.model() * resize2D;

    while(running)
    {
        glfwGetWindowSize(&width, &height);
        height = height > 0 ? height : 1;

        const float cur_aspect = (float)width / (float)height;
        const float resize_ratio = (float)WINSY / (float)height;
        projection2D = glm::ortho(-cur_aspect, cur_aspect, -1.0f, 1.0f, -1.0f, 1.0f);
        resize2D = glm::mat4(1.0f);
        resize2D = glm::scale(resize2D, glm::vec3(resize_ratio, resize_ratio, 1.0f));

        // (begin) this should be an async task
        for(auto iter = str2D_resizeable.begin(); iter != str2D_resizeable.end(); iter++)
        {
            iter->first->set_pos(gl::convert::from_pixels_pos_x(iter->second->pos_x, width) * cur_aspect, gl::convert::from_pixels_pos_y(iter->second->pos_y, height), 0.0f);
            iter->first->clear_model();
            iter->second->mvp = projection2D * iter->first->calc_model("t") * resize2D;
        }
        // (end)

        glViewport(0, 0, width, height);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);

        // draw strings
        text_shader.use();
        for(auto iter = str2D_resizeable.begin(); iter != str2D_resizeable.end(); iter++)
        {
            glBindVertexArray(iter->second->vao);
            //glBindTexture(GL_TEXTURE_2D, iter->first->get_font().get_font_texture_id());
            iter->first->get_font().bind();
            text_shader.uniform_matrix_4x4f("mvp", 1, false, glm::value_ptr(iter->second->mvp));
            glDrawArrays(GL_QUADS, 0, iter->first->vertex_count());
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        text_shader.unuse();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED);
        glfwSwapBuffers();
    }

    for(auto iter = str2D_resizeable.begin(); iter != str2D_resizeable.end(); iter++)
    {
         glDeleteBuffers(1, &iter->second->vbo);
         glDeleteVertexArrays(1, &iter->second->vao);
    }

    glfwTerminate();
    return 0;
}
