/******************************************************************************************************************************************
* Title:        Event handler
* Author:       Michael Reim
* Date:         05.09.2020
* Description:
*   Library to create events and listen to events.
*
* @version release 1.1.0
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* If there are any bugs, contact me!
******************************************************************************************************************************************/

#ifndef __event_h__
#define __event_h__

#include <map>
#include <vector>
#include <list>
#include <chrono>
#include <atomic>

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <thread>
    using std::thread;
#else
    #include <mingw.thread.h>
    using mingw_stdthread::thread;
#endif

enum ListenerType
{
    DYNAMIC_LISTENER,
    STATIC_LISTENER
};

class EventBase
{   
    friend class Listener;

protected:
    virtual bool event_trigger(void) = 0;
    virtual void reset(void) = 0;
public:
    EventBase(void) {/*ctor*/}
    virtual ~EventBase(void) {/*dtor*/}
};

template<typename T>
class Event : public EventBase
{
protected:
    using instance_iterator = std::list<void*>::iterator;

private:
    inline static std::list<void*> instances;
    instance_iterator instance_iter;

protected:
    static const std::list<void*>& get_instances(void)  {return instances;}

public:
    Event(void)
    {
        instances.push_back(this);
        this->instance_iter = instances.end();
        --this->instance_iter;
    }
    virtual ~Event(void)
    {
        instances.erase(this->instance_iter);
    }
};

class Listener
{
    friend class EventHandler;

protected:
    typedef void(*EventFunc)(const EventBase&);

    void set_interval(std::chrono::nanoseconds interval)    {this->listener_interval = interval;}
    void register_event(EventBase& event, EventFunc func)   {this->event2func[&event].push_back(func);}
    virtual void init(void) = 0;

private:
    std::map<EventBase*, std::vector<EventFunc>> event2func;
    thread listener_thread;
    std::atomic_bool running, thread_working;
    std::chrono::nanoseconds listener_interval;

    void start(void);
    void stop(void);
    static void listen(Listener*);

public:
    Listener(void); // listener runs with full speed as standard

    Listener(const Listener&) = delete;
    Listener& operator= (const Listener&) = delete;

    Listener(Listener&&) = delete;
    Listener& operator= (Listener&&) = delete;

    virtual ~Listener(void);
};

class EventHandler
{
private:
    std::vector<Listener*> listeners;
    ListenerType listener_type;
    bool running;

public:
    EventHandler(void);
    EventHandler(ListenerType);

    EventHandler(const EventHandler&) = delete;
    EventHandler& operator= (const EventHandler&) = delete;

    EventHandler(EventHandler&&) = delete;
    EventHandler& operator= (EventHandler&&) = delete;

    virtual ~EventHandler(void);

    void start(void);
    void stop(void);
    void cleanup(void);

    void add_listener(Listener*);
    bool is_running(void) {return this->running;}
};

#endif // __event_h__
