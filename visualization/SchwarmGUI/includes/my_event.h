#ifndef MY_EVENT_H_INCLUDED
#define MY_EVENT_H_INCLUDED

#include <chrono>
#include <Event.h>
#include <cstdio>
#include <vector>
#include "../client/client.h"
#include "../GUI/gui_includes/gui.h"
#include "command.h"

class TextInpListener : public Listener
{
private:
    TextBoxEnterEvent textboxenter;
    inline static Schwarm::Client::SharedSimulationMemory* sharedsimumem;

protected:
    virtual void init(void)
    {
        this->register_event(textboxenter, reinterpret_cast<EventFunc>(on_textenter));
        this->set_interval(std::chrono::milliseconds(5));
    }

public:
    TextInpListener(void)
    {
        this->init();
    }

    static void set_sharedsimumem(Schwarm::Client::SharedSimulationMemory* mem) 
    {
        sharedsimumem = mem;
    }

    static void on_textenter(TextBoxEnterEvent& event)
    {
        std::vector<std::string> args;
        if(Schwarm::decode_command(event.get_text_input().get_text_value(), args))
            Schwarm::on_command(args, sharedsimumem);
        else
            std::cout << get_msg("ERROR") << "A command has to begin with \'/\'." << std::endl;
        event.get_text_input().set_text_value("");
    }
};

#endif // MY_EVENT_H_INCLUDED
