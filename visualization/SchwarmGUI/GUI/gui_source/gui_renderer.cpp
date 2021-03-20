#include "../gui_includes/gui_renderer.h"
#include "../gui_includes/gui_base.h"

using namespace GUI;

void ElementRenderer::renderer_func(ElementRenderer* renderer, std::chrono::nanoseconds interval)
{
    while(renderer->running)
    {
        /* check if buffer must be resized (only for text buffer) */
        const size_t cur_text_maxlength = renderer->get_text_max_length();
        // if text gets bigger allocate memory, if text gets smaller leave memory allocated
        // for deallocation change ">" to "!="
        if(cur_text_maxlength > renderer->text_max_length)
        {
            renderer->text_max_length = cur_text_maxlength;
            renderer->resize_request = true;    // make resize request to OpenGL content thread
            while(renderer->resize_request)     // wait until content thread has finished
            {
                #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
                    std::this_thread::yield();
                #else
                    mingw_stdthread::this_thread::yield();
                #endif
            }

            renderer->load_text_buffer();           // load data into background buffer
            renderer->load_text_command_buffer();   // update text command buffer
            renderer->swap_buffers();               // swap buffers: background buffer -> rendered buffer (foreground buffer)
        }

        /* UPDATE DATA */
        // iterate through every button / write into foreground buffer
        int text_cntr = 0, element_begin_buffer = 0;
        clEnqueueAcquireGLObjects(*renderer->queue, 1, &renderer->element_vertex_buffer, 0, NULL, NULL);
        clEnqueueAcquireGLObjects(*renderer->queue, 1, &renderer->text_vertex_buffer, 0, NULL, NULL);

        for(ElementHandler* h : renderer->handlers)
        {
            for(Element* e : h->handled_elements)
            {
                if(e->mesh_changed())
                {
                    clEnqueueWriteBuffer(*renderer->queue, renderer->element_vertex_buffer, true, element_begin_buffer, e->mesh_buffer_size(), e->generate_mesh(), 0, NULL, NULL);
                }
                if(e->text_mesh_changed() && e->has_text())
                {
                    clEnqueueWriteBuffer(*renderer->queue, renderer->text_vertex_buffer, true, text_cntr * renderer->text_max_length * char_buff_size(), e->text_buff_size(), e->generate_text_mesh(), 0, NULL, NULL);
                    renderer->indirect_commands[text_cntr].vertex_count = e->text_length() * gl::string::char_vertex_count();
                }
                text_cntr += e->has_text();
                element_begin_buffer += e->mesh_buffer_size();
            }
        }

        clEnqueueReleaseGLObjects(*renderer->queue, 1, &renderer->element_vertex_buffer, 0, NULL, NULL);
        clEnqueueReleaseGLObjects(*renderer->queue, 1, &renderer->text_vertex_buffer, 0, NULL, NULL);
        #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
            std::this_thread::sleep_for(interval);
        #else
            mingw_stdthread::this_thread::sleep_for(interval);
        #endif
    }
}

size_t ElementRenderer::get_text_max_length(void)
{
    size_t ret_max_length = min_text_max_length;
    for(ElementHandler* h : this->handlers)
    {
        for(Element* e : h->handled_elements)
        {
            const size_t cur_max_length = (1 + (int)(e->text_length() / min_text_max_length)) * min_text_max_length;
            if(cur_max_length > ret_max_length)
                ret_max_length = cur_max_length;
        }
    }
    return ret_max_length;
}

void ElementRenderer::get_element_info(size_t& element_count, size_t& text_count, size_t& element_vertex_count, size_t& element_buff_size)
{
    element_count = 0;
    element_vertex_count = 0;
    element_buff_size = 0;
    text_count = 0;

    for(ElementHandler* h : this->handlers)
    {
        element_count += h->handled_elements.size();
        for(Element* e : h->handled_elements)
        {
            element_vertex_count += e->vertex_count();
            element_buff_size += e->mesh_buffer_size();
            text_count += e->has_text();
        }
    }
}

void ElementRenderer::init_element_buffer(void)
{
    // create OpenGL and OpenCL buffers / memories
    glGenVertexArrays(1, &this->element_vao);    // button vertex array
    glGenBuffers(1, &this->element_vbo);         // button vertex buffer


    // init button vertex array and buffer
    glBindVertexArray(this->element_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->element_vbo);
    glBufferData(GL_ARRAY_BUFFER, this->element_buff_size, NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, Element::mesh_stride(), (void*)Element::vertex_offset());
    glVertexAttribPointer(1, 4, GL_FLOAT, false, Element::mesh_stride(), (void*)Element::color_offset());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // create OpenCL buffer from VBO
    this->element_vertex_buffer = clCreateFromGLBuffer(*this->context, CL_MEM_READ_WRITE, this->element_vbo, NULL);
}

void ElementRenderer::init_text_buffer(void)
{
    // create OpenGL and OpenCL buffers / memories
    glGenBuffers(2, this->text_vbo);            // text vertex buffers
    glGenVertexArrays(2, this->text_vao);       // text vertex arrays

    // init text vertex arrays and buffers
    for(int i=0; i<2; i++)
    {
        glBindVertexArray(this->text_vao[i]);
        glBindBuffer(GL_ARRAY_BUFFER, this->text_vbo[i]);
        glBufferData(GL_ARRAY_BUFFER, this->text_count * this->text_max_length * char_buff_size(), NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, text_vertex_stride(), (void*)gl::string::vertex_offset());
        glVertexAttribPointer(1, 4, GL_FLOAT, false, text_vertex_stride(), (void*)gl::string::color_offset());
        glVertexAttribPointer(2, 2, GL_FLOAT, false, text_vertex_stride(), (void*)gl::string::tex_offset());
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // use background buffer to load data
    this->text_vertex_buffer = clCreateFromGLBuffer(*this->context, CL_MEM_READ_WRITE, this->text_vbo[this->inactive_text_buffer], NULL);
}

void ElementRenderer::load_text_command_buffer(void)
{
    if(this->indirect_commands == nullptr)
        this->indirect_commands = new DrawArraysIndirectCommand[this->text_count];
    size_t text_cntr = 0;
    for(ElementHandler* h : this->handlers)
    {
        for(Element* e : h->handled_elements)
        {
            if(e->has_text())
            {
                this->indirect_commands[text_cntr].first = text_cntr * this->text_max_length * gl::string::char_vertex_count();
                this->indirect_commands[text_cntr].vertex_count = e->text_length() * gl::string::char_vertex_count();
                this->indirect_commands[text_cntr].instance_count = 1;
                this->indirect_commands[text_cntr].base_instance = 0;
                text_cntr++;
            }
        }
    }
}

void ElementRenderer::load_element_buffer(void)
{
    size_t element_begin_buffer = 0;
    clEnqueueAcquireGLObjects(*this->queue, 1, &this->element_vertex_buffer, 0, NULL, NULL);
    for(ElementHandler* h : this->handlers)
    {
        for(Element* e : h->handled_elements)
        {
            clEnqueueWriteBuffer(*this->queue, this->element_vertex_buffer, true, element_begin_buffer, e->mesh_buffer_size(), e->generate_mesh(), 0, NULL, NULL);
            element_begin_buffer += e->mesh_buffer_size();
        }
    }
    clEnqueueReleaseGLObjects(*this->queue, 1, &this->element_vertex_buffer, 0, NULL, NULL);
}

void ElementRenderer::load_text_buffer(void)
{
    size_t text_cntr = 0;
    clEnqueueAcquireGLObjects(*this->queue, 1, &this->text_vertex_buffer, 0, NULL, NULL);
    for(ElementHandler* h : this->handlers)
    {
        for(Element* e : h->handled_elements)
        {
            if(e->has_text())
            {   
                clEnqueueWriteBuffer(*this->queue, this->text_vertex_buffer, true, text_cntr * this->text_max_length * char_buff_size(), e->text_buff_size(), e->generate_text_mesh(), 0, NULL, NULL);
                ++text_cntr;
            }
        }
    }
    clEnqueueReleaseGLObjects(*this->queue, 1, &this->text_vertex_buffer, 0, NULL, NULL);
}

ElementRenderer::ElementRenderer(cl_context& context, cl_command_queue& queue)
{
    this->running = false;
    this->resize_request = false;
    this->text_max_length = ElementRenderer::min_text_max_length;
    this->inactive_text_buffer = 0;   // use first buffer as first background buffer
    this->indirect_commands = nullptr;
    this->context = &context;
    this->queue = &queue;
}

ElementRenderer::~ElementRenderer(void)
{
    this->stop();
    if(this->element_count > 0)
    {
        // delete cl memories
        clReleaseMemObject(this->element_vertex_buffer);
        clReleaseMemObject(this->text_vertex_buffer);
        // delete vertex arrays
        glDeleteVertexArrays(1, &element_vao);
        glDeleteVertexArrays(2, text_vao);
        // delete buffer objects
        glDeleteBuffers(1, &element_vbo);
        glDeleteBuffers(2, text_vbo);
        // delete text command buffer
        delete(this->indirect_commands);
        this->indirect_commands = nullptr;
    }
}

void ElementRenderer::attach_handler(ElementHandler& handler)
{
    if(!this->running)
        this->handlers.push_back(&handler);
}

void ElementRenderer::init(void)
{
    this->text_max_length = this->get_text_max_length();
    this->get_element_info(this->element_count, this->text_count, this->element_vertex_count, this->element_buff_size);

    if(this->element_count > 0)
    {
        // create buffers
        this->init_element_buffer();
        this->init_text_buffer();
        this->load_text_command_buffer();

        // load data into buffers
        this->load_element_buffer();
        this->load_text_buffer();   // load data into background buffer
        this->swap_buffers();       // swap buffers to use the initialized buffer for rendering
    }
}

void ElementRenderer::update(void)
{
    // check if request-mutex is locked
    if(this->resize_request)
    {
        // use inactive buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->text_vbo[this->inactive_text_buffer]);
        glBufferData(GL_ARRAY_BUFFER, this->text_count * this->text_max_length * char_buff_size(), NULL, GL_DYNAMIC_DRAW);

        clReleaseMemObject(this->text_vertex_buffer);   // delete OpenCL buffer...
        this->text_vertex_buffer = clCreateFromGLBuffer(*this->context, CL_MEM_READ_WRITE, this->text_vbo[this->inactive_text_buffer], NULL);   // ... and bind it to the unused buffer
        this->resize_request = false;
    }
}

void ElementRenderer::start(std::chrono::nanoseconds interval)
{
    if(!this->running)
    {
        this->running = true;
        this->renderer_thread = thread(renderer_func, this, std::chrono::nanoseconds(interval.count()));
    }
}

void ElementRenderer::stop(void)
{
    if(running)
    {
        this->running = false;
        this->renderer_thread.join();
    }
}
