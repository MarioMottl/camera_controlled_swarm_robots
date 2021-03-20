#ifndef TEXTINPUT_H_INCLUDED
#define TEXTINPUT_H_INCLUDED

#include "gui_base.h"
#include <chrono>

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <mutex>
    using std::mutex;
#else
    #include <mingw.mutex.h>
    using mingw_stdthread::mutex;
#endif

namespace GUI
{
    class TextBox : public Element
    {
        using atomic_float = std::atomic<float>;
        using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

    private:
        inline static constexpr size_t  TEXT_BOX_VERTEX_COUNT = 4,
                                        CURSOR_VERTEX_COUNT = 4;

        std::atomic_bool is_active,                 // textbox active status
                         cursor_active;             // cursor active status
        atomic_float pos_x, pos_y,                  // screen position of the textbox
                     size_x, size_y,                // size of the textbox
                     cursor_pos_x, cursor_pos_y;    // screen position of the cursor

        std::atomic_uint cursor_index;           // Position at which character the cursor stands.
        float text_box_color[16], text_color[4], cursor_color[4]; // colors
        gl::string text;                            // graphic string (the string that is redndered)
        std::chrono::milliseconds cursor_interval;  // blink interval of cursor
        float* mesh_buff;                           // buffer for GPU data
        mutex text_box_mutex;                     // mutex because of multiple threads

        // help variables
        time_point_t t0_cursor,         // help value for time measurement for cursor blinking
                     t0_cursor_idle,
                     t0_cursor_repeat;
        int inp_state;
        int old_key;
        std::atomic_bool is_clicked;

        inline float char_ratio(char c);
        inline float char_width(char c);
        void __destruct(void) noexcept;
        void get_cursor_pos(float mouse_x, char& c, std::atomic_uint& index);
        void set_cursor_to_index(size_t index);
        void activate_cursor(bool b) noexcept;
        void process_key_input(int key);

        static int __get_key_code(void) noexcept;
        static char __decode_key_code(int key) noexcept;
        static bool key_blacklisted(int key) noexcept;

    protected:
        virtual bool has_text(void) const noexcept              {return true;}

        virtual const float* generate_mesh(void);
        virtual const float* generate_text_mesh(void);
        virtual const float* mesh(void) const noexcept          {return this->mesh_buff;}
        virtual const float* text_mesh(void) const noexcept     {return this->text.mesh();}

        virtual size_t vertex_count(void) const noexcept        {return TEXT_BOX_VERTEX_COUNT + CURSOR_VERTEX_COUNT;}
        virtual size_t mesh_length(void) const noexcept         {return this->vertex_count() * (Element::color_component_count() + Element::vertex_component_count());}
        virtual size_t mesh_buffer_size(void) const noexcept    {return this->mesh_length() * sizeof(float);}
        virtual size_t text_length(void) const noexcept         {return this->text.length();}
        virtual size_t text_buff_size(void) const noexcept      {return sizeof(float) * this->text.length() * gl::string::char_vertex_count() * gl::string::component_count();}

        virtual void handle(void);

    public:
        TextBox(void) : TextBox(nullptr, nullptr, nullptr) {}
        TextBox(std::atomic_int* width_ptr, std::atomic_int* height_ptr, atomic_float* aspect_ptr);
        TextBox(const TextBox& text_input) = delete;
        virtual ~TextBox(void) {this->__destruct();}

        virtual Type get_type(void) noexcept                    {return Element::Type::TEXT_INPUT;}

        void set_pos_x(float x) noexcept;
        void set_pos_y(float y) noexcept;
        void set_pos(float x, float y) noexcept;
        void set_size_x(float x) noexcept;
        void set_size_y(float y) noexcept;
        void set_size(float x, float y) noexcept;
        void set_textinput_color(const float* color) noexcept;
        void set_text_color(const float* color) noexcept;
        void set_cursor_color(const float* color) noexcept;
        void set_col_space(float space) noexcept;
        void set_font_size(float size) noexcept;
        void set_text_value(const std::string& text);
        void set_cursor_interval(std::chrono::milliseconds interval)    {this->cursor_interval = interval;}

        float get_pos_x(void) const noexcept                    {return this->pos_x;}
        float get_pos_y(void) const noexcept                    {return this->pos_y;}
        void get_pos(float& x, float& y) const noexcept         {x = this->pos_x, y = this->pos_y;}
        float get_size_x(void) const noexcept                   {return this->size_x;}
        float get_size_y(void) const noexcept                   {return this->size_y;}
        void get_size(float& x, float& y) const noexcept        {x = this->size_x; y = this->size_y;}
        float get_col_space(void) const noexcept                {return this->text.col_space();}
        float get_font_size(void) const noexcept                {return this->text.font_size();}
        const std::string& get_text_value(void) const noexcept  {return this->text.std_str();}
        const float* get_text_input_color(void) const noexcept  {return this->text_box_color;}
        const float* get_text_color(void) const noexcept        {return this->text_color;}
        const float* get_cursor_color(void) const noexcept      {return this->cursor_color;}

        bool active(void) const noexcept                        {return this->is_active;}
    };

    struct TextBoxEventInfo
    {
        char c;
        size_t index;
    };

    enum TextBoxAction : int
    {
        TEXT_WRITE          = 0x101,
        TEXT_DELETE         = 0x102,
        TEXT_ENTER          = 0x103,
        TEXT_BOX_ACTIVATE   = 0x104,
        TEXT_BOX_DEACTIVATE = 0x105,
        TEXT_BOX_CLICK      = 0x106
    };
};

#endif // TEXTINPUT_H_INCLUDED
