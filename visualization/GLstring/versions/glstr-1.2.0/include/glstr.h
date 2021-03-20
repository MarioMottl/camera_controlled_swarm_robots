/******************************************************************************************************************************************
* Title:        glstr
* Author:       Michael Reim
* Date:         29.01.2020
* Description:
*   string library for OpenGL to write graphic texts to an OpenGL application
*
* @version release 1.2.0
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* Update Infos for version 1.2.0:
* Changes:
*   - internal changes
*   - added method border() to class string
*
* Bugfixes:
*   - fixed: could not draw more than one string
*
* General:
*   - ---
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
    #ifdef __glstr_internal__
    uint16_t* __internal_get_height(void);
    uint16_t* __internal_get_width(void);
    #endif

    void init(uint16_t sizeX, uint16_t sizeY);

    struct ImageInfo
    {
        uint16_t width;
        uint16_t height;
        uint16_t x_chars;
        uint16_t y_chars;
    };

    struct StringBorder
    {
        uint16_t top_px;
        uint16_t bottom_px;
        uint16_t left_px;
        uint16_t right_px;

        float top;
        float bottom;
        float left;
        float right;
    };

    class Character;

    class Font
    {
    private:
        std::string image_path;
        uint32_t font_texture_id;
        uint16_t image_width, image_height;
        uint16_t x_chars, y_chars;
        bool path_changed;
        Font(const std::string& image_path, uint16_t img_width, uint16_t img_height, uint16_t num_x_chars, uint16_t num_y_chars);
        std::vector<Character> chars;

    public:
        Font(void)
        : Font("", 0 ,0 ,GLSTR_CHAR_WIDTH, GLSTR_CHAR_HEIGHT) {}
        Font(const Font& obj)
        : Font(obj.image_path, obj.image_width, obj.image_height, obj.x_chars, obj.y_chars) {this->chars = obj.chars;}
        Font(const std::string& image_path)
        : Font(image_path, 0, 0, GLSTR_CHAR_WIDTH, GLSTR_CHAR_HEIGHT) {}
        Font(const std::string& image_path, uint16_t num_x_chars, uint16_t num_y_chars)
        : Font(image_path, 0, 0, num_x_chars, num_y_chars) {}

        virtual ~Font(void);

        const std::vector<Character>& get_chars(void) const noexcept;
        const Character& get_char(unsigned char c) const noexcept;
        Character& get_char(unsigned char c) noexcept;
        uint16_t get_image_width(void) const noexcept;
        uint16_t get_image_height(void) const noexcept;
        uint16_t get_font_texture_id(void) const noexcept;
        uint16_t get_x_chars(void) const noexcept;
        uint16_t get_y_chars(void) const noexcept;
        uint32_t get_offset_array_size(void) const noexcept;
        void set_image_path(const std::string& image_path) noexcept;
        void set_num_x_chars(uint32_t num_x_chars) noexcept;
        void set_num_y_chars(uint32_t num_y_chars) noexcept;

        uint8_t load(const uint16_t* const offset_ptr, uint16_t size);
        uint8_t load(const std::vector<uint16_t>* const offset_ptr);
        uint8_t load(void);
    };

    class Character
    {
        friend uint8_t Font::load(const uint16_t* const offset_ptr, uint16_t size);
    private:
        uint16_t char_offsets[4];   // 0: posX, 1: posY, 2: negX, 3: negY
        uint16_t width, height;
        uint16_t char_width, char_height;
        uint16_t pos_on_imgX, pos_on_imgY;
        unsigned char id;
        float tex_coords[8];
        float ignored_tex_coords[8];
        ImageInfo font_img_info;

        float curr_tex_vec[8];
        uint32_t __internal_ignore_1;
        uint32_t __internal_ignore_2;

        void __set(const uint16_t* const char_offsets, unsigned char id, ImageInfo font_img_info);
    public:
        Character(void) : Character(nullptr, 0, {0,0,1,1}) {}
        Character(const uint16_t* const char_offsets, unsigned char id, ImageInfo font_img_info);
        Character(const Character& obj);
        virtual ~Character(void);

        const float* const get_tex_coords(uint32_t ingore1, uint32_t ignore2) noexcept;
        uint16_t get_offset_positive_x(void) const noexcept;
        uint16_t get_offset_positive_y(void) const noexcept;
        uint16_t get_offset_negative_x(void) const noexcept;
        uint16_t get_offset_negative_y(void) const noexcept;
        uint16_t get_width(void) const noexcept;
        uint16_t get_height(void) const noexcept;
        uint16_t get_char_width(void) const noexcept;
        uint16_t get_char_height(void) const noexcept;
        uint16_t get_pos_on_imageX(void) const noexcept;
        uint16_t get_pos_on_imageY(void) const noexcept;
        unsigned char get_id(void) const noexcept;
        void set_offsets(const uint16_t* const char_offsets);
    };

    class string
    {
    private:
        inline static uint32_t shader_program;
        inline static bool shaders_generated = false;
        inline static std::string vertex_shader_program, fragment_shader_program;
        inline static int instance_cntr = 0;


        uint32_t vao, vbo, cbo, tbo;
        std::vector<float> vertices;
        std::vector<float> drawing_colors;
        std::vector<float> texture_coords;

        std::string str;
        std::vector<float> colors;
        Font* font;
        uint16_t font_size;
        uint16_t string_posX, string_posY;
        uint16_t col_space, row_space;
        uint16_t tab_size;
        uint32_t pixel_type;
        StringBorder str_border;

        void __generate_buffers(void);
        void __generate_shaders(void);
        void __generate_vectors(void);
        void __destruct(void);

        #ifdef __glstr_internal__
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
        #endif

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
        const StringBorder& border(void) const noexcept;

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
