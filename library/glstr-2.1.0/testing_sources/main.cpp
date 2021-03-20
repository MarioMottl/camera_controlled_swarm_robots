#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/Shader.h>
#include <GL/glc.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <map>
#include <mingw.thread.h>
#include <mingw.mutex.h>

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
    glVertexAttribPointer(0, 3, GL_FLOAT, false, str.mesh_stride(), (void*)gl::string::vertex_offset());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, str.mesh_stride(), (void*)gl::string::color_offset());
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, str.mesh_stride(), (void*)gl::string::tex_offset());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    data.vbo = vbo;
    data.vao = vao;
}

mingw_stdthread::mutex test_mutex;

void __test(std::atomic_bool* running, gl::string* str)
{
    size_t old_size = str->size();
    while(*running)
    {
        if(str->size() != old_size)
        {
            test_mutex.lock();
            std::cout << str->generate_mesh() << std::endl;
            test_mutex.unlock();
            old_size = str->size();
        }
        mingw_stdthread::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
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
    glfwSwapInterval(0);

    Shader text_shader;
    create_text_shader(text_shader);

    Font console_font;
    gl::FontError error = console_font.load("console_font.png");
    if(error != gl::FontError::FONT_ERROR_NONE)
        return -2;

    std::map<gl::string*, StringData*> str2D_resizeable;

    // init string 1
    gl::string str1("Hello World", console_font, gl::convert::from_pixels_size(50, WINSY));
    StringData str1_data;
    str1_data.pos_x = 0;
    str1_data.pos_y = 0;

    std::vector<float> colors =
    {
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

    std::string text("0");
    std::atomic_bool __running(true);
    mingw_stdthread::thread test_thread(__test, &__running, &str1);
    test_thread.detach();

    while(running)
    {
        if(glfwGetKey(GLFW_KEY_KP_ADD) == GLFW_PRESS)
        {
            text.push_back('0');
            test_mutex.lock();
            str1.set_str(text);
            test_mutex.unlock();
        }
        if(glfwGetKey(GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS && text.size() > 0)
        {
            text.pop_back();
            test_mutex.lock();
            str1.set_str(text);
            test_mutex.unlock();
        }

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
/*

// offsets for specific characters, i would rather create a file and read the offsets from it
    // other characters
    offsets[4*':'+2] = 47;
    offsets[4*'.'+2] = 47;
    // numbers
    offsets[4*'0'+2] = 23;
    offsets[4*'1'+2] = 47;
    offsets[4*'2'+2] = 23;
    offsets[4*'3'+2] = 27;
    offsets[4*'4'+2] = 23;
    offsets[4*'5'+2] = 23;
    offsets[4*'6'+2] = 23;
    offsets[4*'7'+2] = 27;
    offsets[4*'8'+2] = 23;
    offsets[4*'9'+2] = 23;

    //letters A-Z
    offsets[4*'A'+2] = 23;
    offsets[4*'B'+2] = 23;
    offsets[4*'C'+2] = 28;
    offsets[4*'D'+2] = 23;
    offsets[4*'E'+2] = 28;
    offsets[4*'F'+2] = 28;
    offsets[4*'G'+2] = 23;
    offsets[4*'H'+2] = 23;
    offsets[4*'I'+2] = 47;
    offsets[4*'J'+2] = 23;
    offsets[4*'K'+2] = 23;
    offsets[4*'L'+2] = 28;
    offsets[4*'M'+2] = 10;
    offsets[4*'N'+2] = 23;
    offsets[4*'O'+2] = 23;
    offsets[4*'P'+2] = 23;
    offsets[4*'Q'+2] = 23;
    offsets[4*'R'+2] = 23;
    offsets[4*'S'+2] = 23;
    offsets[4*'T'+2] = 19;
    offsets[4*'U'+2] = 23;
    offsets[4*'V'+2] = 23;
    offsets[4*'W'+2] = 10;
    offsets[4*'X'+2] = 24;
    offsets[4*'Y'+2] = 23;
    offsets[4*'Z'+2] = 25;

    offsets[4*'a'+2] = 23;
    offsets[4*'b'+2] = 23;
    offsets[4*'c'+2] = 28;
    offsets[4*'d'+2] = 23;
    offsets[4*'e'+2] = 28;
    offsets[4*'f'+2] = 28;
    offsets[4*'g'+2] = 23;
    offsets[4*'h'+2] = 23;
    offsets[4*'i'+2] = 47;
    offsets[4*'j'+2] = 23;
    offsets[4*'k'+2] = 23;
    offsets[4*'l'+2] = 28;
    offsets[4*'m'+2] = 10;
    offsets[4*'n'+2] = 23;
    offsets[4*'o'+2] = 23;
    offsets[4*'p'+2] = 23;
    offsets[4*'q'+2] = 23;
    offsets[4*'r'+2] = 23;
    offsets[4*'s'+2] = 23;
    offsets[4*'t'+2] = 19;
    offsets[4*'u'+2] = 23;
    offsets[4*'v'+2] = 23;
    offsets[4*'w'+2] = 10;
    offsets[4*'x'+2] = 24;
    offsets[4*'y'+2] = 23;
    offsets[4*'z'+2] = 25;
    */

/*
inline constexpr float pixel_to_screen_sizeX(uint16_t pxCoord)
{
    //return (2.0f * pxCoord - WINSX) / WINSX;
    return pxCoord / (float)WINSX * 2.0f;
}

inline constexpr float pixel_to_screen_sizeY(uint16_t pxCoord)
{
    //return (-2.0f * pxCoord + WINSY) / WINSY;
    return pxCoord / (float)WINSY * 2.0f;
}

inline constexpr float pixel_to_screen_coordX(uint16_t pxCoord)
{
    return (2.0f * pxCoord - WINSX) / WINSX;
}

inline constexpr float pixel_to_screen_coordY(uint16_t pxCoord)
{
    return (-2.0f * pxCoord + WINSY) / WINSY;
}
*/

/*
    const float char_ratio = font.get_char(0).get_width() / (float)font.get_char(0).get_height();
    constexpr uint8_t id = 'g';
    constexpr uint16_t s = 400;
    const float sX = pixel_to_screen_sizeX(s) * char_ratio;
    constexpr float sY = pixel_to_screen_sizeY(s);
    constexpr float bX = pixel_to_screen_coordX(10);
    constexpr float bY = pixel_to_screen_coordY(10);



    float vertices[8] = {
        sX,     0.0f,
        0.0f,   0.0f,
        0.0f,   -sY,
        sX,     -sY

    };

    float transformations[8] = {
        1.0f - (font.get_char(id).get_offset_negative_x() + font.get_char(id).get_offset_positive_x()) / (float)font.get_char(id).get_width(), 1.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f - (font.get_char(id).get_offset_negative_x() + font.get_char(id).get_offset_positive_x()) / (float)font.get_char(id).get_width(), 1.0f
    };

    float color[4] = {1.0f, 0.0f, 1.0f, 1.0f};*/


/*
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, font.get_font_texture_id());
        glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(font.get_char(id).get_tex_coords()[0],font.get_char(id).get_original_tex_coords()[1]);
            glVertex2f(vertices[0] * transformations[0] + bX, vertices[1] * transformations[1] + bY);
            glTexCoord2f(font.get_char(id).get_tex_coords()[2],font.get_char(id).get_original_tex_coords()[3]);
            glVertex2f(vertices[2] * transformations[2] + bX, vertices[3] * transformations[3] + bY);
            glTexCoord2f(font.get_char(id).get_tex_coords()[4],font.get_char(id).get_original_tex_coords()[5]);
            glVertex2f(vertices[4] * transformations[4] + bX, vertices[5] * transformations[5] + bY);
            glTexCoord2f(font.get_char(id).get_tex_coords()[6],font.get_char(id).get_original_tex_coords()[7]);
            glVertex2f(vertices[6] * transformations[6] + bX, vertices[7] * transformations[7] + bY);
        glEnd();
        glDisable(GL_TEXTURE_2D);*/
