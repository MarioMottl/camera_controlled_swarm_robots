#ifndef GUI_HANDLER_H_INCLUDED
#define GUI_HANDLER_H_INCLUDED

#include "gui_base.h"
#include <chrono>
#include <thread>

namespace GUI
{
    class ElementHandler
    {
        friend class ElementRenderer;

    private:
        std::thread handler_thread;
        std::atomic_bool running;
        std::vector<Element*> handled_elements;
        GLFWwindow* window;

        static void handler_func(ElementHandler* handler, std::chrono::nanoseconds interval);

    public:
        ElementHandler(void) = delete;
        ElementHandler(GLFWwindow* window) { this->window = window; }
        ElementHandler(const ElementHandler&) = delete;
        ElementHandler(ElementHandler&&) = delete;
        ElementHandler& operator=(const ElementHandler&) = delete;
        ElementHandler& operator=(ElementHandler&&) = delete;
        
        virtual ~ElementHandler(void);

        void attach_element(Element& element);
        void start(std::chrono::nanoseconds interval = std::chrono::nanoseconds(0));
        void stop(void);
    };
};

#endif // GUI_HANDLER_H_INCLUDED
