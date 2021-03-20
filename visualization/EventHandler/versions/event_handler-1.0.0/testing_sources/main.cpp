#include <iostream>
#include <deque>
#include <atomic>
#include <conio.h>
#include <windows.h>
#include "Event.h"

using namespace std;

class KeyboardEvent : public Event<KeyboardEvent>
{
private:
    inline static deque<atomic_char*> event_data; // queue events if event handler cant keep up
protected:
    virtual bool event_trigger(void) // trigger event if some data is in the queue
    {
        return (event_data.size() > 0);
    }

    virtual void reset(void)    // delete actual event data at reset() that the next event data is at first place
    {
        event_data.pop_front();
    }

public:
    KeyboardEvent(void) {}
    virtual ~KeyboardEvent(void)    // delete all the data
    {
        for(atomic_char* c : event_data)
            delete(c);
    }

    char get_char(void) const // get data (char) of the actual processed event
    {
        return *event_data.at(0);
    }

    static void push(void)  // push data of one event
    {
        event_data.push_back(new atomic_char(getch()));
    }
};

class MyListener : public Listener
{
private:
    KeyboardEvent key_event1, key_event2;   // declare 2 events (KeybortEvent)

    virtual void register_all(void) // register both events with the internal "register_event" method
    {
        // event function must be casted to Listener::EventFunc
        this->register_event(key_event1, reinterpret_cast<Listener::EventFunc>(on_keyboard1));
        this->register_event(key_event2, reinterpret_cast<Listener::EventFunc>(on_keyboard2));
    }
public:

    MyListener(void)    // register all events in the constructor
    {
        register_all();
    }

    static void on_keyboard1(const KeyboardEvent& event)    // on event 1
    {
        cout << "Pressed key (from key event 1): " << event.get_char() << endl;
    }

    static void on_keyboard2(const KeyboardEvent& event)    // on event 2
    {
        cout << "Pressed key (from key event 2): " << event.get_char() << endl;
    }
};

void init_event_handler(EventHandler& handler)
{
    Listener* my_listener = new MyListener();   // create a listener object dynamically

    handler.add_listener(my_listener, 1);   // add listener object to the event handler with an interval of 1ms
}

int main()
{
    EventHandler handler(ListenerType::DYNAMIC_LISTENER);   // is set by default
    init_event_handler(handler);
    handler.start();
    while(!GetAsyncKeyState(VK_ESCAPE))
    {
        if(kbhit())
            KeyboardEvent::push();
    }
    handler.stop();
    handler.cleanup();
    getch();
    return 0;
}
