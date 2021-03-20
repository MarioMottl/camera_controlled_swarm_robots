#include "../gui_includes/textbox.h"
#include "../gui_includes/gui_events.h"

#include <GL/glfw.h>
#include <GL/glc.h>
#include <conio.h>

using namespace GUI;

TextBox::TextBox(std::atomic_int* width_ptr, std::atomic_int* height_ptr, atomic_float* aspect_ptr)
: Element(width_ptr, height_ptr, aspect_ptr)
{
    this->is_active = false;
    this->cursor_active = false;
    this->is_clicked = false;

    this->pos_x = 0.0f;
    this->pos_y = 0.0f;
    this->size_x = 0.0f;
    this->size_y = 0.0f;
    this->cursor_pos_x = (float)this->pos_x;

    for(size_t i=0; i < 16; i++)
        this->text_box_color[i] = 0.0f;

    for(size_t i=0; i < 4; i++)
    {
        this->text_color[i] = 0.0f;
        this->cursor_color[i] = 1.0f;
    }

    this->text.set_font(*Element::get_font());
    this->text.set_color_all(this->text_color[0], this->text_color[1], this->text_color[2], this->text_color[3]);

    this->cursor_interval = std::chrono::seconds(1);
    this->mesh_buff = nullptr;

    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);

    this->inp_state = 0;

    this->t0_cursor = std::chrono::high_resolution_clock::now();
    this->t0_cursor_idle = std::chrono::high_resolution_clock::now();
    this->t0_cursor_repeat = std::chrono::high_resolution_clock::now();
}

inline float TextBox::char_ratio(char c)
{
    return (float)this->text.get_font().at(c).size().char_size_x  / (float)this->text.get_font().at(c).size().char_max_size_y;
}

inline float TextBox::char_width(char c)
{
    return (this->text.font_size() * this->char_ratio(c) + this->text.col_space()) / this->aspect_value();
}

void TextBox::__destruct(void) noexcept
{
    delete(this->mesh_buff);
    this->mesh_buff = nullptr;
}

void TextBox::get_cursor_pos(float mouse_x, char& c, std::atomic_uint& index)
{
    const float border_size = (this->size_y - this->text.font_size()) / 2;  // get the border size of the textbox
    float cur_pos = this->pos_x + border_size;  // position that iterates through the text
    size_t i = 0;                               // index of the current character

    if(mouse_x <= cur_pos)  // exit if border is clicked
    {
        c = 0;
        index = 0;
        return;
    }

    while(cur_pos < mouse_x && i < this->text.size())
    {
        cur_pos += this->char_width(this->text.at(i));
        ++i;
    }

    index = (mouse_x > cur_pos) ? i : i-1;          // after one iteration of the loop i = 1, first index = 0 -> index = i-1
    c = (this->text.size() > 0) ? text.at(i-1) : 0; // character of the index
}

void TextBox::set_cursor_to_index(size_t index)
{
    const float border_size = (this->size_y - this->text.font_size()) / 2;
    float cur_pos = this->pos_x + border_size;

    for(size_t i=0; i<index; i++)
        cur_pos += this->char_width(this->text.at(i));

    this->cursor_pos_x = cur_pos;
}

const float* TextBox::generate_mesh(void)
{
    if(this->mesh_buff == nullptr)
        this->mesh_buff = new float[this->mesh_length()];

    const size_t cursor_offset = this->TEXT_BOX_VERTEX_COUNT * (this->color_component_count() + this->vertex_component_count());

    //             vertex- + colorcomponent
    //             |----------------------|
    // vertex data: x1, y1, r1, g1, b1, a1, x2, y2 ...
    for(size_t v=0; v < 4; v++)
    {
        this->mesh_buff[v * (Element::vertex_component_count() + Element::color_component_count()) + 0] = this->pos_x + ((v == 0) || (v == 3) ? (float)this->size_x : 0.0f); // first vertex component
        this->mesh_buff[v * (Element::vertex_component_count() + Element::color_component_count()) + 1] = this->pos_y + ((v == 2) || (v == 3) ? (float)this->size_y : 0.0f); // second vertex component

        this->mesh_buff[cursor_offset + v * (Element::vertex_component_count() + Element::color_component_count()) + 0] = this->cursor_pos_x + ((v == 0) || (v == 3) ? (float)0.01 : 0.0f);
        this->mesh_buff[cursor_offset + v * (Element::vertex_component_count() + Element::color_component_count()) + 1] = this->pos_y + this->size_y - ((v == 2) || (v == 3) ? (float)0.2f : 0.0f);

        for(size_t c=0; c < Element::color_component_count(); c++)
        {
            this->mesh_buff[v * (Element::vertex_component_count() + Element::color_component_count()) + Element::vertex_component_count() + c] = this->text_box_color[v * Element::color_component_count() + c];
            if(c == 3)
                this->mesh_buff[cursor_offset + v * (Element::vertex_component_count() + Element::color_component_count()) + Element::vertex_component_count() + c] = (this->cursor_active) ? this->cursor_color[c] : 0.0f;
            else
                this->mesh_buff[cursor_offset + v * (Element::vertex_component_count() + Element::color_component_count()) + Element::vertex_component_count() + c] = this->cursor_color[c];
        }
    }
    this->set_mesh_changed(false);
    return this->mesh_buff;
}

const float* TextBox::generate_text_mesh(void)
{
    const float border_size = (this->size_y - this->text.font_size()) / 2;
    this->text.set_fix_pos_x((this->pos_x + border_size) * this->aspect_value());
    this->text.set_fix_pos_y(this->pos_y - border_size + this->size_y);

    this->text_box_mutex.lock();
    this->text.generate_mesh();
    this->text_box_mutex.unlock();

    this->set_text_mesh_changed(false);
    return this->text.mesh();
}

void TextBox::activate_cursor(bool b) noexcept
{
    this->cursor_active = b;
    this->set_mesh_changed(true);
}

void TextBox::process_key_input(int key)
{
    // arrow key actions
    if(key == GLFW_KEY_LEFT && this->cursor_index > 0)
    {
        // force activate cursor if cursor gets moved to left
        this->activate_cursor(true);
        this->t0_cursor = std::chrono::high_resolution_clock::now();    // reset cursor blink time to start at the begin of active cursor

        this->set_cursor_to_index(--this->cursor_index);
        this->set_mesh_changed(true);
    }
    else if(key == GLFW_KEY_RIGHT && this->cursor_index < this->text.size())
    {
        // force activate cursor if cursor gets moved right
        this->activate_cursor(true);
        this->t0_cursor = std::chrono::high_resolution_clock::now();    // reset cursor blink time to start at the begin of active cursor

        this->set_cursor_to_index(++this->cursor_index);
        this->set_mesh_changed(true);
    }
    else if(key == GLFW_KEY_BACKSPACE && this->cursor_index > 0)
    {
        // force activate cursor if chars get deleted
        this->activate_cursor(true);
        this->t0_cursor = std::chrono::high_resolution_clock::now();    // reset cursor blink time to start at the begin of active cursor

        TextBoxActionEvent::push({this->text.at(this->cursor_index-1), this->cursor_index-1});
        ElementActionEvent::push({this, TextBoxAction::TEXT_DELETE});

        this->text.erase(this->cursor_index-1);             // erase character where cursor stands
        this->set_cursor_to_index(--this->cursor_index);    // update cursor position
        this->set_mesh_changed(true);                       // mesh has changed
        this->set_text_mesh_changed(true);                  // text mash has changed
    }
    else if(key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER)
    {
        TextBoxActionEvent::push({0,0});
        ElementActionEvent::push({this, TextBoxAction::TEXT_ENTER});
    }
    else
    {
        char c = __decode_key_code(key);
        if(c != 0 && c != '\t' && c != '\n')
        {
            // force activate cursor if chars get inserted
            this->activate_cursor(true);
            this->t0_cursor = std::chrono::high_resolution_clock::now();    // reset cursor blink time to start at the begin of active cursor

            TextBoxActionEvent::push({c, this->cursor_index});
            ElementActionEvent::push({this, TextBoxAction::TEXT_WRITE});

            this->text.insert(this->cursor_index, c, this->text_color[0], this->text_color[1], this->text_color[2], 1.0f);
            this->set_cursor_to_index(++this->cursor_index);
            this->set_mesh_changed(true);
            this->set_text_mesh_changed(true);
        }
    }
}

int TextBox::__get_key_code(void) noexcept
{
    static int cur_key = 0;
    static std::vector<int> already_pressed_keys;
    std::vector<int> keys;
    for(int i=0; i < GLFW_KEY_LAST; i++)
    {
        if(glfwGetKey(i) == GLFW_PRESS)
            keys.push_back(i);
    }

    if(keys.size() < already_pressed_keys.size())
    {
        already_pressed_keys.clear();
        cur_key = 0;
    }

    for(int k1 : keys)
    {
        bool key_found = false;
        for(int k2 : already_pressed_keys)
        {
            if(k1 == k2)
                key_found = true;
        }
        if(!key_found || already_pressed_keys.size() == 0)
        {
            cur_key = k1;
            already_pressed_keys.push_back(cur_key);
        }
    }

    return cur_key;
}

char TextBox::__decode_key_code(int key) noexcept
{
    const bool shift_state = (glfwGetKey(GLFW_KEY_LSHIFT) || glfwGetKey(GLFW_KEY_RSHIFT));
    const bool ctrl_state = (glfwGetKey(GLFW_KEY_LCTRL) || glfwGetKey(GLFW_KEY_RCTRL));
    const bool alt_state = glfwGetKey(GLFW_KEY_LALT);
    const bool gltgr_state = glfwGetKey(GLFW_KEY_RALT);

    // letters
    if(key >= 'A' && key <= 'Z')
        return ((shift_state) ? key : key + 0x20);

    // numbers
    if(key >= '0' && key <= '9')
    {
        const bool third_char_state = ((ctrl_state && alt_state) || gltgr_state);
        if(!shift_state && !third_char_state)
            return key;
        if(key == '0' && shift_state)       return '=';
        if(key == '0' && third_char_state)  return '}';
        if(key == '1' && shift_state)       return '!';
        if(key == '2' && shift_state)       return '"';
        if(key == '2' && third_char_state)  return 0xFD;
        if(key == '3' && shift_state)       return 0xF5;
        if(key == '3' && third_char_state)  return 0xFC;
        if(key == '4' && shift_state)       return '$';
        if(key == '5' && shift_state)       return '%';
        if(key == '6' && shift_state)       return '&';
        if(key == '7' && shift_state)       return '/';
        if(key == '7' && third_char_state)  return '{';
        if(key == '8' && shift_state)       return '(';
        if(key == '8' && third_char_state)  return '[';
        if(key == '9' && shift_state)       return ')';
        if(key == '9' && third_char_state)  return ']';
    }

    // numpad
    if(key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9)
        return (key - GLFW_KEY_KP_0 + '0');

    if(key == '^')
        return (shift_state) ? 0xF8 : '^';

    if(key == '<')
    {
        if(shift_state)                                     return '>';
        else if(((ctrl_state && alt_state) || gltgr_state)) return '|';
        else                                                return '<';
    }

    if(key == 0xDF)
    {
        if(shift_state)                                     return '?';
        else if(((ctrl_state && alt_state) || gltgr_state)) return '\\';
        else                                                return 0xE1;
    }

    if(key == 0xB4)
        return (shift_state) ? '`' : 0xEF;

    if(key == '+')
    {
        if(shift_state)                                     return '*';
        else if(((ctrl_state && alt_state) || gltgr_state)) return '~';
        else                                                return '+';
    }

    if(key == '#')
        return (shift_state) ? '\'' : '#';

    if(key == '-')
        return (shift_state) ? '_' : '-';

    if(key == '.')
        return (shift_state) ? ':' : '.';

    if(key == ',')
        return (shift_state) ? ';' : ',';

    if(key == GLFW_KEY_KP_DIVIDE)                           return '/';
    if(key == GLFW_KEY_KP_MULTIPLY)                         return '*';
    if(key == GLFW_KEY_KP_SUBTRACT)                         return '-';
    if(key == GLFW_KEY_KP_ADD)                              return '+';
    if(key == GLFW_KEY_KP_DECIMAL)                          return ',';
    if(key == GLFW_KEY_TAB)                                 return '\t';
    if(key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER)   return '\n';
    if(key == GLFW_KEY_SPACE)                               return ' ';

    return 0;
}

bool TextBox::key_blacklisted(int key) noexcept
{
    static constexpr int key_blacklist[7] = {0, GLFW_KEY_LSHIFT, GLFW_KEY_RSHIFT, GLFW_KEY_LCTRL, GLFW_KEY_RCTRL, GLFW_KEY_LALT, GLFW_KEY_LALT};
    for(int i=0; i<7; i++)
    {
        if(key == key_blacklist[i])
            return true;
    }
    return false;
}

void TextBox::handle(void)
{
    int mouse_x, mouse_y;
    if(glfwGetWindowParam(GLFW_OPENED))
        glfwGetMousePos(&mouse_x, &mouse_y);

    float float_mouse_x = gl::convert::from_pixels_pos_x(mouse_x, this->width_value());
    float float_mouse_y = gl::convert::from_pixels_pos_y(mouse_y, this->height_value());

    this->set_last_cursor_pos_x(float_mouse_x);
    this->set_last_cursor_pos_y(float_mouse_y);

    const bool on_box =     float_mouse_x >= this->get_pos_x() &&
                            float_mouse_y >= this->get_pos_y() &&
                            float_mouse_x <= (this->get_pos_x() + this->get_size_x()) &&
                            float_mouse_y <= (this->get_pos_y() + this->get_size_y());

    // activate and deactivate textbox
    if(!this->is_active && on_box && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        this->is_active = true;
        TextBoxActionEvent::push({0, 0});
        ElementActionEvent::push({this, TextBoxAction::TEXT_BOX_ACTIVATE});
    }
    else if(this->is_active && !on_box && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        this->is_active = false;
        this->activate_cursor(false);
        this->is_clicked = false;
        TextBoxActionEvent::push({0, 0});
        ElementActionEvent::push({this, TextBoxAction::TEXT_BOX_DEACTIVATE});
    }
    // set cursor per click
    if(this->is_active && !this->is_clicked && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        char c;
        this->is_clicked = true;
        this->get_cursor_pos(float_mouse_x, c, this->cursor_index);
        this->set_cursor_to_index(this->cursor_index);
        this->set_mesh_changed(true);
        TextBoxActionEvent::push({c, this->cursor_index});
        ElementActionEvent::push({this, TextBoxAction::TEXT_BOX_CLICK});
    }
    else if(this->is_active && this->is_clicked && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
    {
        this->is_clicked = false;
    }

    static constexpr const std::chrono::milliseconds repeat_delay_time(50); // move it 20 times a second (state 2)
    static constexpr const std::chrono::milliseconds idle_time(500);       // wait 1000ms until to switch to state 2

    time_point_t t1_cursor_repeat = std::chrono::high_resolution_clock::now();
    time_point_t t1_cursor_idle = std::chrono::high_resolution_clock::now();

    int key = this->__get_key_code();
    bool key_not_blacklisted = !this->key_blacklisted(key);

    // process first key input async. from 50ms tick
    if(this->is_active && key_not_blacklisted && this->inp_state == 0)
    {
        this->process_key_input(key);
        this->old_key = key;
        this->inp_state = 1;
    }

    if(this->is_active && this->inp_state == 1)
    {
        // reset if key has changed
        if(this->old_key != key)
        {
            this->old_key = key;
            this->inp_state = 0;
        }
        else if((t1_cursor_idle - this->t0_cursor_idle) >= idle_time)
            this->inp_state = 2;
    }

    if(this->is_active && this->inp_state == 2)
    {
        // reset if key has changed
        if(this->old_key != key)
        {
            this->old_key = key;
            this->inp_state = 0;
        }
        else if((t1_cursor_repeat - this->t0_cursor_repeat) >= repeat_delay_time)
        {
            this->process_key_input(key);
            this->t0_cursor_repeat = std::chrono::high_resolution_clock::now();
        }
    }

    if(!this->is_active && !key_not_blacklisted)
        this->inp_state = 0;
    if(this->inp_state == 0)
        this->t0_cursor_idle = std::chrono::high_resolution_clock::now();

    // blink mechanic only operates if textbox is active
    // blink if none of both keys are pressed or if cursor is at the borders
    if(this->is_active && ((glfwGetKey(GLFW_KEY_LEFT) != GLFW_PRESS && glfwGetKey(GLFW_KEY_RIGHT) != GLFW_PRESS) || (this->cursor_index == 0 || this->cursor_index >= this->text.size())))
    {
        // make cursor blink
        time_point_t t1_cursor = std::chrono::high_resolution_clock::now();
        if((t1_cursor - this->t0_cursor) < (this->cursor_interval / 2) && !this->cursor_active)
            this->activate_cursor(true);
        else if((t1_cursor - this->t0_cursor) >= this->cursor_interval / 2 && this->cursor_active)
            this->activate_cursor(false);
        else if((t1_cursor - this->t0_cursor) >= this->cursor_interval)
            this->t0_cursor = std::chrono::high_resolution_clock::now();
    }

    if(this->aspect_changed())
        this->set_text_mesh_changed(true);
}

void TextBox::set_pos_x(float x) noexcept
{
    this->pos_x = x;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void TextBox::set_pos_y(float y) noexcept
{
    this->pos_y = y;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void TextBox::set_pos(float x, float y) noexcept
{
    this->pos_x = x;
    this->pos_y = y;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void TextBox::set_size_x(float x) noexcept
{
    this->size_x = x;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void TextBox::set_size_y(float y) noexcept
{
    this->size_y = y;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void TextBox::set_size(float x, float y) noexcept
{
    this->size_x = x;
    this->size_y = y;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void TextBox::set_textinput_color(const float* color) noexcept
{
    this->text_box_mutex.lock();
    memcpy(this->text_box_color, color, 16 * sizeof(float));
    this->text_box_mutex.unlock();
    this->set_mesh_changed(true);
}

void TextBox::set_text_color(const float* color) noexcept
{
    this->text_box_mutex.lock();
    memcpy(this->text_color, color, 4 * sizeof(float));
    this->text_box_mutex.unlock();
    this->text.set_color_all(color[0], color[1], color[2], color[3]);
    this->set_text_mesh_changed(true);
}

void TextBox::set_cursor_color(const float* color) noexcept
{
    this->text_box_mutex.lock();
    memcpy(this->cursor_color, color, 4 * sizeof(float));
    this->text_box_mutex.unlock();
    this->set_mesh_changed(true);
}

void TextBox::set_col_space(float space) noexcept
{
    this->text.set_col_space(space);
    this->set_text_mesh_changed(true);
}

void TextBox::set_font_size(float size) noexcept
{
    this->text.set_font_size(size);
    this->set_text_mesh_changed(true);
}

void TextBox::set_text_value(const std::string& text)
{
    this->text_box_mutex.lock();
    this->text.set_str(text);
    this->text_box_mutex.unlock();
    this->cursor_index = this->text.size();
    this->set_cursor_to_index(this->cursor_index);
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}
