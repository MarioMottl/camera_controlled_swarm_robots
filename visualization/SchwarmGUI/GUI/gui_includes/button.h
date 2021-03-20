#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "gui_base.h"

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <mutex>
    using std::mutex;
#else
    #include <mingw.mutex.h>
    using mingw_stdthread::mutex;
#endif

namespace GUI
{
    class Button : public Element
    {
        using atomic_float = std::atomic<float>;
    private:

        atomic_float pos_x, pos_y, size_x, size_y;
        float color[16];        // 4 vertices * 4 color components
        float text_color[4];    // 4 color components
        gl::string button_text;
        float* mesh_buff;
        mutex button_mutex;

        std::atomic_bool __is_hovered,
                        __is_left_clicked,
                        __is_right_clicked;


        void __destruct(void);

        inline float char_ratio(char);
        void text_dimensons(float& width, float& height);

        virtual bool has_text(void) const noexcept                  {return true;}

        virtual const float* generate_mesh(void);
        virtual const float* generate_text_mesh(void);
        virtual const float* mesh(void) const noexcept              {return this->mesh_buff;}
        virtual const float* text_mesh(void) const noexcept         {return this->button_text.mesh();}

        virtual size_t vertex_count(void) const noexcept            {return 4;}
        virtual size_t mesh_length(void) const noexcept             {return this->vertex_count() * (Button::vertex_component_count() + Button::color_component_count());}
        virtual size_t mesh_buffer_size(void) const noexcept        {return this->mesh_length() * sizeof(float);}
        virtual size_t text_length(void) const noexcept             {return this->button_text.length();}
        virtual size_t text_buff_size(void) const noexcept          {return sizeof(float) * this->text_length() * gl::string::char_vertex_count() * gl::string::component_count();}

        virtual void handle(void);

    public:

        Button(void);
        Button(std::atomic_int* width_ptr, std::atomic_int* height_ptr, atomic_float* aspect_ptr);
        Button(const Button& button) = delete;
        virtual ~Button(void) {__destruct();}

        virtual Type get_type(void) noexcept    {return Element::Type::BUTTON;}

        void set_pos_x(float pos_x) noexcept;
        void set_pos_y(float pos_y) noexcept;
        void set_pos(float pos_x, float pos_y) noexcept;
        void set_size_x(float size_x) noexcept;
        void set_size_y(float size_y) noexcept;
        void set_size(float size_x, float size_y) noexcept;
        void set_button_color(const float* color) noexcept;
        void set_text_color(const float* color) noexcept;
        void set_text_value(const std::string& text) noexcept;
        void set_text_col_space(float space) noexcept;
        void set_text_row_space(float space) noexcept;
        void set_font_size(float size) noexcept;

        float get_pos_x(void) const noexcept                            {return this->pos_x;}
        float get_pos_y(void) const noexcept                            {return this->pos_y;}
        void get_pos(float& pos_x, float& pos_y) const noexcept         {pos_x = this->pos_x; pos_y = this->pos_y;}         
        float get_size_x(void) const noexcept                           {return this->size_x;}
        float get_size_y(void) const noexcept                           {return this->size_y;}
        void get_size(float& size_x, float& size_y) const noexcept      {size_x = this->size_x; size_y = this->size_y;}
        const float* get_button_color(void) const noexcept              {return this->color;}
        const float* get_text_color(void) const noexcept                {return this->text_color;}
        const std::string& get_text_value(void) const noexcept          {return this->button_text.std_str();}
        float get_text_col_space(void) const noexcept                   {return this->button_text.col_space();}
        float get_text_row_space(void) const noexcept                   {return this->button_text.row_space();}
        float get_text_size(void) const noexcept                        {return this->button_text.font_size();}
    };

    struct ButtonEventInfo
    {
        float mouse_x;
        float mouse_y;
    };

    enum ButtonAction : unsigned int
    {
        NONE                = 0x001,
        HOVER_ON            = 0x002,
        HOVER_OFF           = 0x003,
        CLICK_LEFT_DOWN     = 0x004,
        CLICK_LEFT_UP       = 0x005,
        CLICK_RIGHT_DOWN    = 0x006,
        CLICK_RIGHT_UP      = 0x007
    };
};

#endif