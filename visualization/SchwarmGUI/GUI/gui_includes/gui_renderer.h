#ifndef GUI_RENDERER_H_INCLUDED
#define GUI_RENDERER_H_INCLUDED

#include "gui_handler.h"
#include <GL/glew.h>
#include <CL/opencl.h>

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <thread>
    using std::thread;
#else
    #include <mingw.thread.h>
    using mingw_stdthread::thread;
#endif

namespace GUI
{
    class ElementRenderer
    {
    private:
        struct DrawArraysIndirectCommand
        {
            unsigned int vertex_count;
            unsigned int instance_count;
            unsigned int first; // vertex offset
            unsigned int base_instance;
        };

        thread renderer_thread;
        std::atomic_bool running, resize_request;
        std::vector<ElementHandler*> handlers;
        cl_context* context;
        cl_command_queue* queue;

        static constexpr size_t min_text_max_length = 10;  // min. buffer size = 10 characters
        size_t text_max_length, element_count, element_vertex_count, element_buff_size, text_count;

        unsigned int element_vao, text_vao[2];
        unsigned int element_vbo, text_vbo[2];
        DrawArraysIndirectCommand* indirect_commands;
        cl_mem element_vertex_buffer, text_vertex_buffer;
        std::atomic_uint inactive_text_buffer;

        static void renderer_func(ElementRenderer* renderer, std::chrono::nanoseconds interval);

        inline static size_t char_buff_size(void)       {return gl::string::component_count() * gl::string::char_vertex_count() * sizeof(float);}
        inline static size_t text_vertex_stride(void)   {return gl::string::component_count() * sizeof(float);}

        size_t get_text_max_length(void);
        void get_element_info(size_t& element_count, size_t& text_count, size_t& element_vertex_count, size_t& element_buff_size);
        void init_element_buffer(void);
        void init_text_buffer(void);
        void load_text_command_buffer(void);
        void load_element_buffer(void);
        void load_text_buffer(void);

        inline void swap_buffers(void)                  {this->inactive_text_buffer = (this->inactive_text_buffer == 0) ? 1 : 0;}

    public:
        ElementRenderer(void) = delete;
        ElementRenderer(const ElementRenderer&) = delete;
        ElementRenderer(ElementRenderer&&) = delete;
        ElementRenderer& operator=(const ElementRenderer&) = delete;
        ElementRenderer& operator=(ElementRenderer&&) = delete;

        ElementRenderer(cl_context& context, cl_command_queue& queue);
        virtual ~ElementRenderer(void);

        void attach_handler(ElementHandler& handler);
        void init(void);
        void update(void);
        void start(std::chrono::nanoseconds interval = std::chrono::nanoseconds(0));
        void stop(void);

        inline unsigned int get_element_vertex_array(void)   {return this->element_vao;}
        inline unsigned int get_element_vertex_count(void)   {return this->element_vertex_count;}
        inline unsigned int get_text_vertex_array(void)     {return this->text_vao[(this->inactive_text_buffer == 0) ? 1 : 0];}
        inline void* get_text_command(void)                 {return this->indirect_commands;}
        inline size_t get_text_count(void)                  {return this->text_count;}  // text count = button count
        inline size_t get_text_command_stride(void)         {return sizeof(DrawArraysIndirectCommand);}
    };
};

#endif // GUI_RENDERER_H_INCLUDED
