#include "../gui_includes/button.h"
#include "../gui_includes/gui_events.h"
#include <GL/glfw.h>
#include <GL/glc.h>

using namespace GUI;

inline float Button::char_ratio(char c)
{
    return (float)this->button_text.get_font().at(c).size().char_size_x  / (float)this->button_text.get_font().at(c).size().char_max_size_y;
}

void Button::text_dimensons(float& width, float& height)
{
    width = 0.0f;
    height = 0.0f;

    float cur_width = 0.0f;

    for(size_t i=0; i < this->button_text.size(); i++)
    {
        if(this->button_text.at(i) == '\n')
            height += this->button_text.font_size() + this->button_text.row_space();
        else if(this->button_text.at(i) == '\r')
        {
            width = (cur_width > width) ? cur_width : width;
            cur_width = 0.0f;
        }
        else if(this->button_text.at(i) == '\t')
            cur_width = cur_width + this->button_text.tab_size() - fmod(cur_width, this->button_text.tab_size());
        else
            cur_width += this->button_text.font_size() * char_ratio(this->button_text.at(i)) + this->button_text.col_space();

    }
    width = (cur_width > width) ? cur_width : width;
    height += this->button_text.font_size();
}

void Button::__destruct(void)
{
    delete(this->mesh_buff);
    this->mesh_buff = nullptr;
}

const float* Button::generate_mesh(void)
{
    if(this->mesh_buff == nullptr)
        this->mesh_buff = new float[this->mesh_length()];

    for(size_t v=0; v < this->vertex_count(); v++)
    {
        this->mesh_buff[v * (this->vertex_component_count() + this->color_component_count()) + 0] = this->pos_x + ((v == 0 || v == 3) ? (float)this->size_x : 0.0f);
        this->mesh_buff[v * (this->vertex_component_count() + this->color_component_count()) + 1] = this->pos_y + ((v == 2 || v == 3) ? (float)this->size_y : 0.0f);

        for(size_t c=0; c < this->color_component_count(); c++)
            this->mesh_buff[v * (this->vertex_component_count() + this->color_component_count()) + this->vertex_component_count() + c] = this->color[v * this->color_component_count() + c];
    }
    this->set_mesh_changed(false);
    return this->mesh_buff;
}

const float* Button::generate_text_mesh(void)
{
    float tsx, tsy;
    this->text_dimensons(tsx, tsy);

    this->button_text.set_fix_pos_x((this->pos_x + this->size_x / 2) * aspect_value() - tsx / 2);
    this->button_text.set_fix_pos_y(this->pos_y + this->size_y / 2 + tsy / 2);

    this->button_mutex.lock();
    this->button_text.generate_mesh();
    this->button_mutex.unlock();

    this->set_text_mesh_changed(false);
    return this->button_text.mesh();
}

Button::Button(void)
: Button(nullptr, nullptr, nullptr) {}

Button::Button(std::atomic_int* width_ptr, std::atomic_int* height_ptr, atomic_float* aspect_ptr)
: Element(width_ptr, height_ptr, aspect_ptr)
{
    // init button
    this->pos_x = 0.0f;
    this->pos_y = 0.0f;
    this->size_x = 0.0f;
    this->size_y = 0.0f;
    for(size_t i=0; i<16; i++)
        this->color[i] = 0.0f;

    //init button text
    for(size_t i=0; i<4; i++)
        this->text_color[i] = 0.0f;
    this->button_text.set_font(*Element::get_font());
    this->button_text.set_color_all(this->text_color[0], this->text_color[1], this->text_color[2], this->text_color[3]);

    // init button event values
    this->__is_hovered = false;
    this->__is_right_clicked = false;
    this->__is_left_clicked = false;

    // generate meshes
    this->mesh_buff = nullptr;
    this->generate_mesh();
    this->generate_text_mesh();
}

void Button::set_pos_x(float pos_x) noexcept
{
    this->pos_x = pos_x;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);  // regenerate text because the text position depends on the button's position, same for the size
}

void Button::set_pos_y(float pos_y) noexcept
{
    this->pos_y = pos_y;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void Button::set_pos(float pos_x, float pos_y) noexcept
{
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void Button::set_size_x(float size_x) noexcept
{
    this->size_x = size_x;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void Button::set_size_y(float size_y) noexcept
{
    this->size_y = size_y;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void Button::set_size(float size_x, float size_y) noexcept
{
    this->size_x = size_x;
    this->size_y = size_y;
    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void Button::set_button_color(const float* color) noexcept
{
    this->button_mutex.lock();
    memcpy(this->color, color, 16 * sizeof(float));
    this->button_mutex.unlock();
    this->set_mesh_changed(true);
}

void Button::set_text_color(const float* color) noexcept
{
    this->button_mutex.lock();
    memcpy(this->text_color, color, 4 * sizeof(float));
    this->button_mutex.unlock();
    this->button_text.set_color_all(this->text_color[0], this->text_color[1], this->text_color[2], this->text_color[3]);
    this->set_text_mesh_changed(true);
}

void Button::set_text_value(const std::string& text) noexcept
{
    this->button_mutex.lock();
    this->button_text.set_str(text);
    this->button_mutex.unlock();
    this->set_text_mesh_changed(true);
}

void Button::set_text_col_space(float space) noexcept
{
    this->button_text.set_col_space(space);
    this->set_text_mesh_changed(true);
}

void Button::set_text_row_space(float space) noexcept
{
    this->button_text.set_row_space(space);
    this->set_text_mesh_changed(true);
}

void Button::set_font_size(float size) noexcept
{
    this->button_text.set_font_size(size);
    this->set_text_mesh_changed(true);
}

void Button::handle(void)
{
    int mouse_x, mouse_y;
    if(glfwGetWindowParam(GLFW_OPENED))
        glfwGetMousePos(&mouse_x, &mouse_y);

    float float_mouse_x = gl::convert::from_pixels_pos_x(mouse_x, this->width_value());
    float float_mouse_y = gl::convert::from_pixels_pos_y(mouse_y, this->height_value());

    this->set_last_cursor_pos_x(float_mouse_x);
    this->set_last_cursor_pos_y(float_mouse_y);

    const bool on_button =  float_mouse_x >= this->get_pos_x() &&
                            float_mouse_y >= this->get_pos_y() &&
                            float_mouse_x <= (this->get_pos_x() + this->get_size_x()) &&
                            float_mouse_y <= (this->get_pos_y() + this->get_size_y());

    if(on_button && !this->__is_hovered)   // hover on event
    {
        ButtonActionEvent::push({float_mouse_x, float_mouse_y});
        ElementActionEvent::push({this, ButtonAction::HOVER_ON});
        this->__is_hovered = true;
    }
    if(!on_button && this->__is_hovered)   // hover off event
    {
        ButtonActionEvent::push({float_mouse_x, float_mouse_y});
        ElementActionEvent::push({this, ButtonAction::HOVER_OFF});
        this->__is_hovered = false;

        if(this->__is_left_clicked)
        {
            ButtonActionEvent::push({float_mouse_x, float_mouse_y});
            ElementActionEvent::push({this, ButtonAction::CLICK_LEFT_UP});
            this->__is_left_clicked = false;
        }
        if(this->__is_right_clicked)
        {
            ButtonActionEvent::push({float_mouse_x, float_mouse_y});
            ElementActionEvent::push({this, ButtonAction::CLICK_RIGHT_UP});
            this->__is_right_clicked = false;
        }
    }
    if(on_button && !this->__is_left_clicked && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))    // left click down event
    {
        ButtonActionEvent::push({float_mouse_x, float_mouse_y});
        ElementActionEvent::push({this, ButtonAction::CLICK_LEFT_DOWN});
        this->__is_left_clicked = true;
    }
    if(on_button && this->__is_left_clicked && !glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))    // left click up event
    {
        ButtonActionEvent::push({float_mouse_x, float_mouse_y});
        ElementActionEvent::push({this, ButtonAction::CLICK_LEFT_UP});
        this->__is_left_clicked = false;
    }
    if(on_button && !this->__is_right_clicked && glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))  // right click down event
    {
        ButtonActionEvent::push({float_mouse_x, float_mouse_y});
        ElementActionEvent::push({this, ButtonAction::CLICK_RIGHT_DOWN});
        this->__is_right_clicked = true;
    }
    if(on_button && this->__is_right_clicked && !glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))  // right click up event
    {
        ButtonActionEvent::push({float_mouse_x, float_mouse_y});
        ElementActionEvent::push({this, ButtonAction::CLICK_RIGHT_UP});
        this->__is_right_clicked = false;
    }

    if(this->aspect_changed())
        this->set_text_mesh_changed(true);
}