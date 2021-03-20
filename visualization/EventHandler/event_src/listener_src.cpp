#include "../Event.h"

Listener::Listener(void)
{
    this->running = false;
    this->thread_working = false;
    this->listener_interval = std::chrono::nanoseconds(0);
}

Listener::~Listener(void)
{
    this->stop();
}

void Listener::start(void)
{
    if(!this->running)
    {
        this->running = true;
        this->thread_working = true;
        this->listener_thread = thread(listen, this);
        this->listener_thread.detach();
    }
}

void Listener::stop(void)
{
    if(this->running)
    {
        this->running = false;
        while(this->thread_working);
    }
}

void Listener::listen(Listener* l)
{
    while(l->running)
    {
        for(auto iter = l->event2func.begin(); iter != l->event2func.end(); iter++) // gothrough every event
        {
            if(iter->first->event_trigger())    // if event is triggered, execute all event functions for this object
            {
                for(EventFunc efunc : iter->second)
                    efunc(*iter->first);    // execute event func
            }
            iter->first->reset();       // reset event after call every event func
        }
        mingw_stdthread::this_thread::sleep_for(l->listener_interval);
    }
    l->thread_working = false;
}
