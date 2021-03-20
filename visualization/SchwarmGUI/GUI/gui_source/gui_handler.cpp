#include "../gui_includes/gui_handler.h"
#include "../gui_includes/gui_base.h"

#include <GL/glfw.h>
#include <GL/glc.h>

void GUI::ElementHandler::handler_func(ElementHandler* handler, std::chrono::nanoseconds interval)
{
    while(handler->running)
    {
        int mouse_x, mouse_y;
        if(glfwGetWindowParam(GLFW_OPENED))
            glfwGetMousePos(&mouse_x, &mouse_y);

        for(Element* e : handler->handled_elements)
        {
            e->handle();
        }

        #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
            std::this_thread::sleep_for(interval);
        #else
            mingw_stdthread::this_thread::sleep_for(interval);
        #endif
        }
    }

    GUI::ElementHandler::ElementHandler(void)
    {
        this->running = false;
    }

    GUI::ElementHandler::~ElementHandler(void)
    {
        this->stop();
        this->handled_elements.clear();
    }

    void GUI::ElementHandler::attach_element(Element& element)
    {
        if(!this->running)
            this->handled_elements.push_back(&element);
    }

    void GUI::ElementHandler::start(std::chrono::nanoseconds interval)
    {
        if(!this->running)
        {
            this->running = true;
            this->handler_thread = thread(handler_func, this, std::chrono::nanoseconds(interval.count()));
        }
    }

    void GUI::ElementHandler::stop(void)
    {
        if(this->running)
        {
            this->running = false;
            this->handler_thread.join();
        }
    }