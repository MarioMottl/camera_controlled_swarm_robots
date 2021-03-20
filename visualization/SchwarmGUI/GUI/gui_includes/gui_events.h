#ifndef GUI_EVENTS_H_INCLUDED
#define GUI_EVENTS_H_INCLUDED

#include "gui_base.h"
#include "gui_handler.h"
#include "button.h"
#include "textbox.h"
#include <Event.h>
#include <deque>

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <mutex>
    using std::mutex;
#else
    #include <mingw.mutex.h>
    using mingw_stdthread::mutex;
#endif

using namespace GUI;

/* ---------- ELEMENT EVENTS ---------- */

class ElementActionEvent : public Event<ElementActionEvent>
{
    friend class GUI::Button;
    friend class GUI::TextBox;

private:
    mutex event_mutex;

    static void push(GUI::ElementEventInfo info)
    {
        for(void* instance : get_instances())
        {
            ElementActionEvent* event = reinterpret_cast<ElementActionEvent*>(instance);
            if(event->event_queue.size() < 16)
            {
                event->event_mutex.lock();
                event->event_queue.push_back(info);
                event->event_mutex.unlock();
            }
        }
    }
protected:
    std::deque<GUI::ElementEventInfo> event_queue;
    int event_id;

    virtual bool event_trigger(void)    {return this->event_queue.size() > 0;}
    virtual void reset(void)
    {
        if(this->event_queue.size() > 0)
        {
            this->event_mutex.lock();
            this->event_queue.pop_front();
            this->event_mutex.unlock();
        }
    }

public:
    ElementActionEvent(void)                    {this->event_id = 0;}

    GUI::Element& get_element(void)             {return *this->event_queue.at(0).element;}
    int get_action(void)                        {return this->event_queue.at(0).action;}
};

/* ---------- BUTTON EVENTS ---------- */

class ButtonActionEvent : public ElementActionEvent
{
    friend class GUI::Button;

private:
    mutex event_mutex;

    static void push(GUI::ButtonEventInfo info)
    {
        for(void* instance : get_instances())
        {
            ButtonActionEvent* event = reinterpret_cast<ButtonActionEvent*>(instance);
            if(event->buttononly_event_queue.size() < 16 && event->event_id == 1)
            {
                event->event_mutex.lock();
                event->buttononly_event_queue.push_back(info);
                event->event_mutex.unlock();
            }
        }
    }

protected:
    std::deque<GUI::ButtonEventInfo> buttononly_event_queue;

    virtual bool event_trigger(void)
    {
        return (this->event_queue.size() > 0 && this->buttononly_event_queue.size() > 0) ? true : false;
    }

    virtual void reset(void)
    {
        if(this->event_queue.size() > 0)
        {
            this->event_mutex.lock();
            if(this->buttononly_event_queue.size() > 0)
                this->buttononly_event_queue.pop_front();
            this->event_queue.pop_front();
            this->event_mutex.unlock();
        }
    }


public:
    ButtonActionEvent(void)         {this->event_id = 1;}

    GUI::Element& get_element(void) = delete;
    GUI::Button& get_button(void)   {return *((GUI::Button*)this->event_queue.at(0).element);}
    float get_mouse_x(void)         {return this->buttononly_event_queue.at(0).mouse_x;}
    float get_mouse_y(void)         {return this->buttononly_event_queue.at(0).mouse_y;}
};

class ButtonInteractEvent : public ButtonActionEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0 && this->buttononly_event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                action == GUI::ButtonAction::HOVER_ON ||
                action == GUI::ButtonAction::HOVER_OFF ||
                action == GUI::ButtonAction::CLICK_LEFT_DOWN ||
                action == GUI::ButtonAction::CLICK_LEFT_UP ||
                action == GUI::ButtonAction::CLICK_RIGHT_DOWN ||
                action == GUI::ButtonAction::CLICK_RIGHT_UP
            )
            {
                return true;
            }
        }
        return false;
    }
};

class ButtonHoverEvent : public ButtonInteractEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0 && this->buttononly_event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                action == GUI::ButtonAction::HOVER_ON ||
                action == GUI::ButtonAction::HOVER_OFF
            )
            {
                return true;
            }
        }
        return false;
    }
};

class ButtonClickEvent : public ButtonInteractEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0 && this->buttononly_event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                action == GUI::ButtonAction::CLICK_LEFT_DOWN ||
                action == GUI::ButtonAction::CLICK_LEFT_UP ||
                action == GUI::ButtonAction::CLICK_RIGHT_DOWN ||
                action == GUI::ButtonAction::CLICK_RIGHT_UP
            )
            {
                return true;
            }
        }
        return false;
    }
};

/* ---------- TEXT INPUT EVENTS ---------- */

class TextBoxActionEvent : public ElementActionEvent
{
    friend class GUI::TextBox;

private:
    mutex event_mutex;

    static void push(GUI::TextBoxEventInfo info)
    {
        for(void* instance : get_instances())
        {
            TextBoxActionEvent* event = reinterpret_cast<TextBoxActionEvent*>(instance);  // cast void* ptr to ptr to super class
            if(event->tionly_event_queue.size() < 16 && event->event_id == 2)
            {
                event->event_mutex.lock();
                event->tionly_event_queue.push_back(info);
                event->event_mutex.unlock();
            }
        }
    }
    
protected:
    std::deque<GUI::TextBoxEventInfo> tionly_event_queue;

    virtual bool event_trigger(void)
    {
        return (this->event_queue.size() > 0 && this->tionly_event_queue.size() > 0) ? true : false;
    }

    virtual void reset(void)
    {
        if(this->event_queue.size() > 0)
        {
            this->event_mutex.lock();
            if(this->tionly_event_queue.size() > 0)
                this->tionly_event_queue.pop_front();
            this->event_queue.pop_front();
            this->event_mutex.unlock();
        }
    }

public:
    TextBoxActionEvent(void)              {this->event_id = 2;}

    GUI::Element& get_element(void) = delete;
    GUI::TextBox& get_text_input(void)      {return *((GUI::TextBox*)this->event_queue.at(0).element);}
    char get_char(void)                     {return this->tionly_event_queue.at(0).c;}
    size_t get_index(void)                  {return this->tionly_event_queue.at(0).index;}
};

class TextBoxEditEvent : public TextBoxActionEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0 && this->tionly_event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                action == GUI::TextBoxAction::TEXT_WRITE ||
                action == GUI::TextBoxAction::TEXT_DELETE
            )
            {
                return true;
            }
        }
        return false;
    }
};

class TextBoxEnterEvent : public TextBoxActionEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0 && this->tionly_event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                action == GUI::TextBoxAction::TEXT_ENTER
            )
            {
                return true;
            }
        }
        return false;
    } 
public:
    char get_char(void)     = delete;
    size_t get_index(void)  = delete;
};

class TextBoxActivateEvent : public TextBoxActionEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0 && this->tionly_event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                action == GUI::TextBoxAction::TEXT_BOX_ACTIVATE ||
                action == GUI::TextBoxAction::TEXT_BOX_DEACTIVATE
            )
            {
                return true;
            }
        }
        return false;
    } 
};

class TextBoxInteractEvent : public TextBoxActionEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                this->event_queue.at(0).element->get_type() == GUI::Element::Type::TEXT_INPUT &&
                (
                    action == GUI::TextBoxAction::TEXT_BOX_CLICK
                )
            )
            {
                return true;
            }
        }
        return false;
    } 
};

#endif // GUI_EVENTS_H_INCLUDED
