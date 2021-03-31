#include "../gui_includes/gui_handler.h"
#include "../gui_includes/gui_base.h"

#include <GL/glc.h>

void GUI::ElementHandler::handler_func(ElementHandler* handler, std::chrono::nanoseconds interval)
{
    while(handler->running)
    {
        for(Element* e : handler->handled_elements)
        {
            e->handle();
        }
        std::this_thread::sleep_for(interval);
    }
}

GUI::ElementHandler::ElementHandler(GLFWwindow* window)
{
    this->window = window;
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
        this->handler_thread = std::thread(handler_func, this, std::chrono::nanoseconds(interval.count()));
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