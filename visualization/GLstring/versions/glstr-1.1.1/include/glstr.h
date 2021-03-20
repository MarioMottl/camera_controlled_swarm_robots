/******************************************************************************************************************************************
* Title:        glstr
* Author:       Michael Reim
* Date:         26.01.2020
* Description:
*   string library for OpenGL to write graphic texts on an OpenGL application
*
* @version release 1.1.1
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* Update Infos for version 1.1.1:
* Changes:
*   - added clear method to glstr::string, there has been no option to clear the string before
*   - clear is similar to .set_str("")
*   - added weak_generate to glstr::string, does the same as generate but it does not update the drawing data
*
* Bugfixes:
*   - ---
*
* General:
*   - multi-threading compatible
*
* If there are any bugs, contact me!
******************************************************************************************************************************************/

#ifndef GLSTR_H_INCLUDED
#define GLSTR_H_INCLUDED
#define __glstr_h__

#ifndef __glew_h__
    #error glew.h requiered
#endif // __glew_h__
#ifndef __glfw_h_
    #error glfw.h requiered
#endif // __glfw_h_

#ifdef GL_VERSION_4_2

#include <string>
#include <vector>

#define GLSTR_CHAR_WIDTH    16
#define GLSTR_CHAR_HEIGHT   16
#define GLSTR_NONE          0x0
#define GLSTR_BLACK         0x1
#define GLSTR_WHITE         0x2
#define GLSTR_SCREEN_PIXEL  0x3
#define GLSTR_IMAGE_PIXEL   0x4
#define GLSTR_IGNORE_X      0x5
#define GLSTR_IGNORE_Y      0x6

namespace glstr
{
    uint16_t* __internal_get_height(void);
    uint16_t* __internal_get_width(void);

    void init(uint16_t sizeX, uint16_t sizeY);    // init window width and height

    class FontBase
    {
    protected:
        std::string image_path;
        uint32_t font_texture_id;
        uint16_t image_width, image_height;
        uint16_t x_chars, y_chars;
        bool path_changed;

        FontBase(const std::string& image_path, uint16_t img_width, uint16_t img_height, uint16_t num_x_chars, uint16_t num_y_chars);
        virtual ~FontBase(void);
    public:
        uint16_t get_image_width(void) const noexcept
        {return this->image_width;}

        uint16_t get_image_height(void) const noexcept
        {return this->image_height;}

        uint16_t get_font_texture_id(void) const noexcept
        {return this->font_texture_id;}

        uint16_t get_x_chars(void) const noexcept
        {return this->x_chars;}

        uint16_t get_y_chars(void) const noexcept
        {return this->y_chars;}

        uint32_t get_offset_array_size(void) const noexcept
        {return this->x_chars * this->y_chars * 4;}

        void set_image_path(const std::string& image_path) noexcept
        {this->image_path = image_path; this->path_changed = true;}

        void set_num_x_chars(uint32_t num_x_chars) noexcept
        {this->x_chars = num_x_chars;}

        void set_num_y_chars(uint32_t num_y_chars) noexcept
        {this->y_chars = num_y_chars;}
    };

    class Character
    {
    private:
        uint16_t char_offsets[4];   // 0: posX, 1: posY, 2: negX, 3: negY
        uint16_t width, height;
        uint16_t char_width, char_height;
        uint16_t pos_on_imgX, pos_on_imgY;
        unsigned char id;
        FontBase* belonging_font;
        float tex_coords[8];
        float ignored_tex_coords[8];

        float curr_tex_vec[8];
        uint32_t __internal_ignore_1;
        uint32_t __internal_ignore_2;

    public:
        Character(void) : Character(nullptr, 0, nullptr) {}
        Character(const uint16_t* const char_offsets, unsigned char id, FontBase* belonging_font);
        Character(const Character& obj);
        virtual ~Character(void);

        void set(const uint16_t* const char_offsets, unsigned char id, FontBase* belonging_font);

        // returns the texture coordinates (4* s,t), offsets included
        const float* const get_tex_coords(uint32_t ingore1, uint32_t ignore2) noexcept;

        // returns positive-X-offset
        uint16_t get_offset_positive_x(void) const noexcept
        {return this->char_offsets[0];}

        // returns positive-Y-offset
        uint16_t get_offset_positive_y(void) const noexcept
        {return this->char_offsets[1];}

        // returns negative-X-offset
        uint16_t get_offset_negative_x(void) const noexcept
        {return this->char_offsets[2];}

        // returns negative-Y-offset
        uint16_t get_offset_negative_y(void) const noexcept
        {return this->char_offsets[3];}

        // returns pixel-character-width without offsets included
        uint16_t get_width(void) const noexcept
        {return this->width;}

        // returns pixel-character-height without offsets included
        uint16_t get_height(void) const noexcept
        {return this->height;}

        // returns pixel-character-width with offsets included
        uint16_t get_char_width(void) const noexcept
        {return this->char_width;}

        // returns pixel-character-width width offsets included
        uint16_t get_char_height(void) const noexcept
        {return this->char_height;}

        // returns pixel-position-X on font image
        uint16_t get_pos_on_imageX(void) const noexcept
        {return this->pos_on_imgX;}

        // returns pixel-position-Y on font image
        uint16_t get_pos_on_imageY(void) const noexcept
        {return this->pos_on_imgY;}

        // returns id of character
        unsigned char get_id(void) const noexcept
        {return this->id;}

        void set_offsets(const uint16_t* const char_offsets);
    };

    class Font : public FontBase
    {
    private:
        std::vector<Character> chars;
    public:
        Font(void)
        : FontBase("", 0 ,0 ,GLSTR_CHAR_WIDTH, GLSTR_CHAR_HEIGHT)
        {this->chars.resize(this->x_chars * this->y_chars);}

        Font(const Font& obj)
        : FontBase(obj.image_path, obj.image_width, obj.image_height, obj.x_chars, obj.y_chars)
        {this->chars = obj.chars;
        this->chars.resize(this->x_chars * this->y_chars);}

        Font(const std::string& image_path)
        : FontBase(image_path, 0, 0, GLSTR_CHAR_WIDTH, GLSTR_CHAR_HEIGHT)
        {this->chars.resize(this->x_chars * this->y_chars);}

        Font(const std::string& image_path, uint16_t num_x_chars, uint16_t num_y_chars)
        : FontBase(image_path, 0, 0, num_x_chars, num_y_chars)
        {this->chars.resize(this->x_chars * this->y_chars);}

        virtual ~Font(void)
        {this->~FontBase();}

        const std::vector<Character>& get_chars(void) const noexcept
        {return this->chars;}

        const Character& get_char(unsigned char c) const noexcept
        {return this->chars[c];}

        Character& get_char(unsigned char c) noexcept
        {return this->chars[c];}

        uint8_t load(const uint16_t* const offset_ptr, uint16_t size);

        uint8_t load(const std::vector<uint16_t>* const offset_ptr)
        {return this->load((offset_ptr == nullptr) ? nullptr : &(*offset_ptr)[0], (uint16_t)offset_ptr->size());}

        uint8_t load(void)
        {return this->load(nullptr, 0);}
    };

    class string
    {
    private:
        inline static uint32_t shader_program;
        inline static bool shaders_generated = false;
        inline static std::string vertex_shader_program, fragment_shader_program;

        uint32_t vao, vbo, cbo, tbo;
        std::vector<float> vertices;
        std::vector<float> drawing_colors;
        std::vector<float> texture_coords;

        std::string str;
        std::vector<float> colors;
        Font* font;
        uint16_t font_size; // schriftgröße
        uint16_t string_posX, string_posY;
        uint16_t col_space, row_space;
        uint16_t tab_size;
        uint32_t pixel_type;

        void __generate_buffers(void);
        void __generate_shaders(void);
        void __generate_vectors(void);
        void __destruct(void);

        inline static float pixel_to_screen_sizeX(uint16_t pxCoord)
        {
            //return (2.0f * pxCoord - WINSX) / WINSX;
            return pxCoord / (float)*__internal_get_width() * 2.0f;
        }
        inline static float pixel_to_screen_sizeY(uint16_t pxCoord)
        {
            //return (-2.0f * pxCoord + WINSY) / WINSY;
            return pxCoord / (float)*__internal_get_height() * 2.0f;
        }
        inline static float pixel_to_screen_coordX(uint16_t pxCoord)
        {
            return (2.0f * pxCoord - *__internal_get_width()) / *__internal_get_width();
        }
        inline static float pixel_to_screen_coordY(uint16_t pxCoord)
        {
            return (-2.0f * pxCoord + *__internal_get_height()) / *__internal_get_height();
        }

    public:
        string(void);
        string(const std::string& str, Font* font, uint16_t font_size);
        string(const std::string& str, Font* font, uint16_t font_size, uint32_t fill_color);
        string(const std::string& str, Font* font, uint16_t font_size, const std::vector<float>& colors);
        string(const std::string& str, Font* font, uint16_t font_size, const std::vector<float>& colors, uint32_t fill_color);
        string(const string& obj);
        virtual ~string(void);

        const Font& get_font(void) const noexcept;
        const Font* get_font_ptr(void) const noexcept;
        const std::vector<float>& get_colors(void) const noexcept;
        uint16_t get_font_size(void) const noexcept;
        uint16_t get_col_space(void) const noexcept;
        uint16_t get_row_space(void) const noexcept;
        uint16_t get_str_posX(void) const noexcept;
        uint16_t get_str_posY(void) const noexcept;
        uint16_t get_tab_size(void) const noexcept;
        uint32_t get_pixel_type(void) const noexcept;
        const std::string& std_str(void) const noexcept;
        const char* const c_str(void) const noexcept;
        char at(uint32_t index) const;
        const float* const at_color(uint32_t index) const;
        uint32_t length(void) const noexcept;
        uint32_t size(void) const noexcept;

        void set_str(const std::string& str);
        void set_str(const std::string& str, uint32_t fill_color);
        void set_color(float* color4f);
        void set_color(uint32_t index, float* color4f);
        void set_colors(const std::vector<float>& colors);
        void set_colors(const std::vector<float>& colors, uint32_t fill_color);
        void set_font(Font* font) noexcept;
        void set_font_size(uint16_t font_size) noexcept;
        void set_col_space(uint16_t col_space) noexcept;
        void set_row_space(uint16_t row_space) noexcept;
        void set_str_posX(uint16_t posX) noexcept;
        void set_str_posY(uint16_t posY) noexcept;
        void set_str_tab_size(uint16_t t_size) noexcept;
        void set_pixel_type(uint32_t pixel_type) noexcept;
        void set(const std::string& str, Font* font, uint16_t font_size);
        void set(const std::string& str, Font* font, uint16_t font_size, uint32_t fill_color);
        void set(const std::string& str, Font* font, uint16_t font_size, const std::vector<float>& colors);
        void set(const std::string& str, Font* font, uint16_t font_size, const std::vector<float>& colors, uint32_t fill_color);
        void set(const string& obj);
        void clear(void);

        void push_front(char c);
        void push_front(char c, float* color4f);
        void push_back(char c);
        void push_back(char c, float* color4f);
        char pop_front(void);
        char pop_back(void);
        void insert(uint32_t index, char c);
        void insert(uint32_t index, char c, float* color4f);
        void erase(uint32_t index_begin, uint32_t index_end);
        char replace(uint32_t index, char _new_c);
        void fill(uint32_t index_begin, uint32_t index_end, char _new_c);
        void fill(uint32_t index_begin, uint32_t index_end, const char* const _new_chars);
        string& copy(const string& obj) noexcept;
        string& reverse_copy(const string& obj) noexcept;
        string& reverse(void) noexcept;
        string& add(const string& obj);
        string& add(const string& obj, uint32_t fill_color);

        int32_t find(char c) const noexcept;    // returns index of first matching character, -1 if not found
        bool compare(const string& obj) const noexcept;
        bool compare_string(const string& obj) const noexcept;
        bool compare_font(const string& obj) const noexcept;
        bool compare_colors(const string& obj) const noexcept;

        void weak_generate(void);
        void generate(void);
        void update(void);
        void draw(void) const;

        string& operator= (const string& obj) noexcept;
        string& operator+= (const string& obj) noexcept;
        bool operator== (const string& obj) const noexcept;
        bool operator!= (const string& obj) const noexcept;
        bool operator> (const string& obj) const noexcept;
        bool operator< (const string& obj) const noexcept;
        bool operator>= (const string& obj) const noexcept;
        bool operator<= (const string& obj) const noexcept;
        char operator[] (uint32_t index) const;
    };
};
#endif // GL_VERSION_4_2

#endif // GLSTR_H_INCLUDED
