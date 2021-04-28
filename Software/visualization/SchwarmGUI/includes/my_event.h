#ifndef MY_EVENT_H_INCLUDED
#define MY_EVENT_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <event.h>
#include <cstdio>
#include <vector>
#include "../client/client.h"
#include "../GUI/gui_includes/gui.h"
#include "command.h"

class TextInpListener : public Listener
{
private:
    TextBoxEnterEvent textboxenter;
    inline static std::map<Schwarm::Client::ClientType, Schwarm::Client::SharedMemory>* shared_memory;

protected:
    virtual void init(void)
    {
        this->register_event(textboxenter, reinterpret_cast<EventFunc>(on_textenter));
    }

public:
    TextInpListener(void)
    {
        this->init();
    }

    static void set_shared_memory(std::map<Schwarm::Client::ClientType, Schwarm::Client::SharedMemory>* mem)
    {
        shared_memory = mem;
    }

    static void on_textenter(TextBoxEnterEvent& event)
    {
        std::vector<std::string> args;
        if(Schwarm::decode_command(event.get_text_input().get_text_value(), args))
            Schwarm::on_command(args, shared_memory);
        else
            std::cout << get_msg("ERROR") << "A command has to begin with \'/\'." << std::endl;
        event.get_text_input().set_text_value("");
    }
};

#endif // MY_EVENT_H_INCLUDED
