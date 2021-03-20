#ifndef GUI_HANDLER_H_INCLUDED
#define GUI_HANDLER_H_INCLUDED

#include "gui_base.h"
#include <chrono>
#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <thread>
    using std::thread;
#else
    #include <mingw.thread.h>
    using mingw_stdthread::thread;
#endif

namespace GUI
{
    class ElementHandler
    {
        friend class ElementRenderer;

    private:
        thread handler_thread;
        std::atomic_bool running;
        std::vector<Element*> handled_elements;

        static void handler_func(ElementHandler* handler, std::chrono::nanoseconds interval);

    public:
        ElementHandler(void);
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
