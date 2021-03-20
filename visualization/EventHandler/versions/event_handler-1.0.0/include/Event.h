/******************************************************************************************************************************************
* Title:        Event handler
* Author:       Michael Reim
* Date:         14.08.2020
* Description:
*   Library to create events and listen to events.
*
* @version release 1.0.0
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* If there are any bugs, contact me!
******************************************************************************************************************************************/

#ifndef __event_h__
#define __event_h__

#include <map>
#include <vector>
#include <atomic>
#include <mingw.thread.h>

enum EventHandlerStatus
{
    HANDLER_ON,
    HANDLER_OFF
};

enum ListenerType
{
    DYNAMIC_LISTENER,
    STATIC_LISTENER
};

class EventBase
{
    friend class Listener;
protected:
    // only for internal purposes.
    virtual void internal_reset(void) = 0;

    // Method to indicate when the event should be called.
    virtual bool event_trigger(void) = 0;

    // Method that is used to reset the event.
    virtual void reset(void) = 0;
public:
    EventBase(void) {}
    virtual ~EventBase(void) {}
};

template<typename T>
class Event : public EventBase
{
private:
    inline static size_t    instance_cntr = 0,
                            event_cntr = 0;

    void internal_reset(void)
    {
        if(event_cntr == 0)
            event_cntr = instance_cntr;
        --event_cntr;
        if(event_cntr == 0)
            this->reset();  // call reset if every event has been processed
    }

protected:
    Event(void)             {++instance_cntr;}
    virtual ~Event(void)    {--instance_cntr;}
};

class Listener
{
    friend class EventHandler;
protected:
    typedef void(*EventFunc)(const EventBase&);

    // Registers an event to the listener.
    // @param event: the event object that should be registrated.
    // @param func: the event function that should be called.
    void register_event(EventBase& event, EventFunc func);

    // Method that is used to register all events and their functions.
    virtual void register_all(void) = 0;
private:
    std::map<EventBase*, EventFunc> event_functions;
    void listen(void);
public:
    Listener(void) {}

    Listener(const Listener&) = delete;
    Listener& operator= (const Listener&) = delete;

    Listener(Listener&&) = delete;
    Listener& operator= (Listener&&) = delete;

    virtual ~Listener(void) {}
};

class EventHandler
{
private:
    std::vector<Listener*> listeners;
    std::map<Listener*, mingw_stdthread::thread> listener_threads;
    std::map<Listener*, uint16_t> listener_intervals;
    std::atomic<EventHandlerStatus> handler_status;
    ListenerType listener_type;

    static void async_listen(Listener* listener, uint16_t interval, std::atomic<EventHandlerStatus>& status);

public:
    EventHandler(void);

    // Sets whether dynamic or static listener objects are used. Dynamic by default.
    EventHandler(ListenerType);

    EventHandler(const EventHandler&) = delete;
    EventHandler& operator= (const EventHandler&) = delete;

    EventHandler(EventHandler&&) = delete;
    EventHandler& operator= (EventHandler&&) = delete;

    // Starts the event handler and a thread gets created for each added listener.
    void start(void);

    // Stops the event handler and shuts down every running listener thread.
    void stop(void);

    // Deletes all added listeners and clears all the internal data containers.
    void cleanup(void);

    // Returnes the actual listener status.
    EventHandlerStatus status(void);

    // Adds a new listener to the event handler.
    // @param listener: listener object itself.
    // @param internal: the listen interval in ms.
    void add_listener(Listener* listener, uint16_t interval);
};

#endif // __event_h__
