#ifndef WINDOW_HANLDER_H_INCLUDED
#define WINDOW_HANLDER_H_INCLUDED

#include <GL/glfw.h>
#include <atomic>

class WindowHandler
{
    using atomic_float = std::atomic<float>;

private:
    std::atomic_int win_width = 0, win_height = 1, start_height = 1;
    atomic_float win_aspect = 1.0f, size_aspect = 1.0f;

public:
    WindowHandler(void)                     {this->init(0, 1);}
    WindowHandler(int width, int height)    {this->init(width, height);}
    virtual ~WindowHandler(void)    {/*dtor*/}

    void init(int width, int height) noexcept
    {
        this->win_width = width;
        this->win_height = height;
        this->start_height = height;

        this->win_aspect = (float)width / (float)height;
        this->size_aspect = 1.0f;
    }

    int get_width(void) const noexcept                  {return this->win_width;}
    std::atomic_int* get_width_ptr(void) noexcept       {return &this->win_width;}
    int get_height(void) const noexcept                 {return this->win_height;}
    std::atomic_int* get_height_ptr(void) noexcept      {return &this->win_height;}
    float get_aspect(void) const noexcept               {return this->win_aspect;}
    atomic_float* get_aspect_ptr(void) noexcept         {return &this->win_aspect;}
    float get_size_aspect(void) const noexcept          {return this->size_aspect;}
    atomic_float* get_size_aspect_ptr(void) noexcept    {return &this->size_aspect;}

    void update(int width, int height, bool set_window_size) noexcept
    {
        if(width != win_width || height != this->win_height)
        {
            height = (height < 1) ? 1 : height;
            this->win_aspect = (float)width / (float)height;
            this->size_aspect = (float)start_height / (float)height;
            this->win_width = width;
            this->win_height = height;
            if(set_window_size)
                glfwSetWindowSize(width, height);
        }
    }
};

#endif // WINDOW_HANLDER_H_INCLUDED
