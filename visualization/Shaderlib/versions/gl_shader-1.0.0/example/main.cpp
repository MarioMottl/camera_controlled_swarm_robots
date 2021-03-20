#include <GL/glew.h>
#include <GL/glfw.h>
#include <iostream>

#include "Shader.h"

int main()
{
    int win_width = 1920 / 2, win_height = 1080 / 2;
    int win_posx = 1920 / 4, win_posy = 1080 / 4;

    glfwInit();
    if(!glfwOpenWindow(win_width, win_height, 8, 8, 8, 8, 32, 32, GLFW_WINDOW))
    {
        std::cout << "Could not open window." << std::endl;
        return -1;
    }
    glewInit();
    glfwSetWindowPos(win_posx, win_posy);
    glfwSetWindowTitle("Test program for shader library");
    glfwSwapInterval(1);

    gl::Shader shader;
    gl::ShaderLoadError error = shader.load("test_vertex_shader.txt", "test_fragment_shader.txt");

    if(error & gl::ShaderLoadErrorType::VERTEX_SHADER_ERROR)
        std::cout << "Vertex Shader: " << shader.get_last_vertex_msg() << std::endl;
    if(error & gl::ShaderLoadErrorType::FRAGMENT_SHADER_ERROR)
        std::cout << "Fragment Shader: " << shader.get_last_fragment_msg() << std::endl;
    if(error & gl::ShaderLoadErrorType::SHADER_LINK_ERROR)
        std::cout << "Linker: " << shader.get_last_link_msg() << std::endl;

    float quad_vertices[12] = {
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    // generate buffer objects
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    // generate vertex array objects
    unsigned int vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3*sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while(!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED))
    {
        glfwGetWindowSize(&win_width, &win_height);
        // if window height is 0 it will cause division by 0
        if(win_height < 1)
        {
            win_height = 1;
            glfwSetWindowSize(win_width, win_height);
        }

        glViewport(0, 0, win_width, win_height);
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.uniform_4f("obj_color", 0.0f, 0.0f, 1.0f, 1.0f);
        glBindVertexArray(vao);
        glDrawArrays(GL_QUADS, 0, sizeof(quad_vertices) / 3);
        glBindVertexArray(0);
        shader.unuse();

        glfwSwapBuffers();
    }

    glfwTerminate();
    return 0;
}
