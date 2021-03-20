#include <iostream>
#include <deque>
#include <conio.h>
#include <chrono>
#include <windows.h>
#include "Event.h"

using namespace std;

class KeyEvent : public Event<KeyEvent>
{
private:
    std::deque<char> event_queue;

protected:
    virtual bool event_trigger(void)
    {
        return (this->event_queue.size() > 0);
    }

    virtual void reset(void)
    {
        if(this->event_queue.size() > 0)
            this->event_queue.pop_front();
    }

public:
    char get_char(void) {return this->event_queue.at(0);}

    static void push(char c)
    {
        for(void* instance : get_instances())
        {
            KeyEvent* event = (KeyEvent*)instance;
            event->event_queue.push_back(c);
        }
    }
};

class MyListener : public Listener
{
private:
    KeyEvent key_event1;

protected:
    virtual void init(void)
    {
        this->register_event(key_event1, (Listener::EventFunc)on_keybd1);
        this->register_event(key_event1, (Listener::EventFunc)on_keybd2);
        this->set_interval(std::chrono::milliseconds(5));
    }

public:
    MyListener(void)
    {
        this->init();
    }

    static void on_keybd1(KeyEvent& event)
    {
        std::cout << "From Listener1 / Function 1: " << event.get_char() << std::endl;
    }

    static void on_keybd2(KeyEvent& event)
    {
        std::cout << "From Listener1 / Function 2: " << event.get_char() << std::endl;
    }
};

class MyListener2 : public Listener
{
private:
    KeyEvent key_event1;

protected:
    virtual void init(void)
    {
        this->register_event(key_event1, (Listener::EventFunc)on_keybd1);
        this->register_event(key_event1, (Listener::EventFunc)on_keybd2);
        this->set_interval(std::chrono::milliseconds(5));
    }

public:
    MyListener2(void)
    {
        this->init();
    }

    static void on_keybd1(KeyEvent& event)
    {
        std::cout << "From Listener2 / Function 1: " << event.get_char() << std::endl;
    }

    static void on_keybd2(KeyEvent& event)
    {
        std::cout << "From Listener2 / Function 2: " << event.get_char() << std::endl;
    }
};

void init_event_handler(EventHandler& handler)
{
    Listener* my_listener = new MyListener();
    Listener* my_listener2 = new MyListener2();
    handler.add_listener(my_listener);
    handler.add_listener(my_listener2);
}

int main()
{
    EventHandler event_handler;
    init_event_handler(event_handler);
    event_handler.start();

    while(!GetAsyncKeyState(VK_ESCAPE))
    {
        if(kbhit())
            KeyEvent::push(getch());
        Sleep(5);
    }

    event_handler.stop();
    event_handler.cleanup();
    getch();
    getch();
    return 0;
}
