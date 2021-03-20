#ifndef GUI_BASE_H_INCLUDED
#define GUI_BASE_H_INCLUDED

#include <GL/glstr.h>
#include <atomic>
#include <vector>

namespace GUI
{
    class Element
    {
        friend class ElementHandler;
        friend class ElementRenderer;
        using atomic_float = std::atomic<float>;

    private:
        inline static gl::Font* element_font;

        std::atomic_bool __mesh_changed, 
        /* I don't use the internal mesh_changed functuon because it detects only changes with the string itself, 
         * but I also want to regenerate the string when the aspect of the window changes. 
         * If the aspect has changes, the positions of the string will have to be recalculated. Therefore, I can't use the built-in mesh_changed function. */
                        __text_mesh_changed;

        atomic_float __last_cursor_x,
                     __last_cursor_y,
                     __last_aspect,
                     __last_width,
                     __last_height;

        atomic_float* __aspect_ptr = nullptr;

        std::atomic_int* __width_ptr = nullptr,
                    * __height_ptr = nullptr;
    public:
        enum Type
        {
            BUTTON,
            TEXT_INPUT
        };

    protected:
        bool width_changed(void) noexcept;
        bool height_changed(void) noexcept;
        bool aspect_changed(void) noexcept;

        virtual bool has_text(void) const noexcept              = 0;

        virtual const float* generate_mesh(void)                = 0;
        virtual const float* generate_text_mesh(void)           = 0;
        virtual const float* mesh(void) const noexcept          = 0;
        virtual const float* text_mesh(void) const noexcept     = 0;

        virtual size_t mesh_length(void) const noexcept         = 0;
        virtual size_t mesh_buffer_size(void) const noexcept    = 0;
        virtual size_t vertex_count(void) const noexcept        = 0;
        virtual size_t text_length(void) const noexcept         = 0;
        virtual size_t text_buff_size(void) const noexcept      = 0;

        virtual void handle(void)                               = 0;

        bool mesh_changed(void) const noexcept                  {return this->__mesh_changed;}
        bool text_mesh_changed(void) const noexcept             {return this->__text_mesh_changed;}
        void set_mesh_changed(bool b) noexcept                  {this->__mesh_changed = b;}
        void set_text_mesh_changed(bool b) noexcept             {this->__text_mesh_changed = b;}

        inline float aspect_value(void)                         {return (__aspect_ptr == nullptr) ? 1.0f : (float)*__aspect_ptr;}
        inline int width_value(void)                            {return (__width_ptr == nullptr) ? 1 : (int)*__width_ptr;}
        inline int height_value(void)                           {return (__height_ptr == nullptr) ? 1 : (int)*__height_ptr;}

        float last_cursor_pos_x(void)                           {return this->__last_cursor_x;}
        float last_cursor_pos_y(void)                           {return this->__last_cursor_y;}
        void set_last_cursor_pos_x(float f)                     {this->__last_cursor_x = f;}
        void set_last_cursor_pos_y(float f)                     {this->__last_cursor_y = f;}

    public:
        Element(void) : Element(nullptr, nullptr, nullptr) {}
        Element(std::atomic_int* width_ptr, std::atomic_int* height_ptr, atomic_float* aspect_ptr);
        virtual ~Element(void) {/*dtor*/}

        virtual Type get_type(void) noexcept = 0;

        void set_aspect_ptr(atomic_float* ptr)                  {this->__aspect_ptr = ptr;}
        void set_width_ptr(std::atomic_int* ptr)                {this->__width_ptr = ptr;}
        void set_height_ptr(std::atomic_int* ptr)               {this->__height_ptr = ptr;}

        static size_t vertex_component_count(void) noexcept     {return 2;}
        static size_t color_component_count(void) noexcept      {return 4;}

        static size_t mesh_stride(void) noexcept                {return (vertex_component_count() + color_component_count()) * sizeof(float);}
        static size_t vertex_offset(void) noexcept              {return 0;}
        static size_t color_offset(void) noexcept               {return vertex_component_count() * sizeof(float);}

        static void set_font(gl::Font* font) noexcept           {element_font = font;}
        static gl::Font* get_font(void) noexcept                {return element_font;}
    };

    struct ElementEventInfo
    {
        Element* element;
        int action;
    };
};

#endif // GUI_BASE_H_INCLUDED
