/******************************************************************************************************************************************
* Title:        glstr
* Author:       Michael Reim
* Date:         14.08.2020
* Description:
*   Library to implement strings and texts on an OpenGL graphic scene.
*
* @version release 2.0.0
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* If there are any bugs, contact me!
******************************************************************************************************************************************/

#ifndef GLSTR_H_INCLUDED
#define GLSTR_H_INCLUDED
#define __glstr_h__

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <atomic>

namespace gl
{
    struct ImageInfo
    {
        uint16_t width{0};
        uint16_t height{0};
        uint16_t x_chars{0};
        uint16_t y_chars{0};
    };

    struct CharacterOffset
    {
        uint16_t positive_x{0};
        uint16_t positive_y{0};
        uint16_t negative_x{0};
        uint16_t negative_y{0};
    };

    struct CharacterSizeInfo
    {
        // actual size of a character (size with offsets included)
        uint16_t char_size_x{0};
        uint16_t char_size_y{0};
        // maximum size of a character (size without offsets included)
        uint16_t char_max_size_x{0};
        uint16_t char_max_size_y{0};
    };

    struct CharacterTexCoords
    {
        // texture coordinates in right order
        float top_right_x{0.0f};
        float top_right_y{0.0f};

        float top_left_x{0.0f};
        float top_left_y{0.0f};

        float bottom_left_x{0.0f};
        float bottom_left_y{0.0f};

        float bottom_right_x{0.0f};
        float bottom_right_y{0.0f};
    };

    enum FontError
    {
        font_error_none,
        font_error_invalid_path,
        font_error_invalid_image_size,
        font_error_invalid_num_chars
    };

    enum StringType
    {
        STRING_2D,
        STRING_2D_DISTANCE_FIELD,
        STRING_3D,
        STRING_3D_DISTANCE_FIELD
    };

    class Character;

    /******************************************************************************************************************************************
    *   Class: Font
    *
    *   Description: Loads a font-image-file and splits it into an array of single characters. The data of the characters
    *                is contained by the Character-class. Font contains also information of the image-file itself: with, height
    *                and numboer of characters in X and Y direction.
    *
    ******************************************************************************************************************************************/

    class Font
    {
    private:
        uint32_t font_texture_id;
        ImageInfo img_info;
        std::vector<Character> chars;

    public:
        Font(uint16_t num_x_chars = 16, uint16_t num_y_chars = 16);
        Font(const Font& obj);
        Font(Font&& obj);
        virtual ~Font(void);

        const Character& get_char(unsigned char c) const;
        Character& get_char(unsigned char c);
        uint32_t get_font_texture_id(void) const noexcept;
        const ImageInfo& get_image_info(void) const noexcept;
        uint16_t get_num_chars(void) const noexcept;
        void set_num_chars(uint16_t num_x_chars, uint16_t num_y_chars);

        FontError load(const std::string& image_path, bool linear_filter = false, const std::vector<CharacterOffset>& offsets = {});
        void bind(void) const;
        void unbind(void) const;

        Font& operator= (const Font& obj);
        Font& operator= (Font&& obh);
    };

    /******************************************************************************************************************************************
    *   Class: Character
    *
    *   Description: It stores data of a single character of the font-image-file.
    *                It calculates the texture-coordinates, position on the image and also it's own width and height (also with offsets included).
    *                For that, it requieres the ImageInfo-struct, character-ID and character-offsets as input.
    *
    *   Info: This class is NOT there for drawing a single character in the graphic scene.
    *         It's ONLY purpose is to calculate and store data and information about position, size and texture-coordinates
    *         of one single character.
    *
    ******************************************************************************************************************************************/

    class Character
    {
    private:
        unsigned char id;
        uint16_t pos_on_imgX, pos_on_imgY;
        CharacterOffset offset;
        CharacterSizeInfo size_info;
        CharacterTexCoords tex_coords;
        CharacterTexCoords normal_tex_coords;

    public:
        Character(void);
        Character(const CharacterOffset& offset, unsigned char id);
        Character(const Character& obj);
        Character(Character&& obj);
        virtual ~Character(void);

        void set_id(unsigned char id) noexcept;
        const CharacterTexCoords& get_texture_coordinates(void) const noexcept;
        const CharacterTexCoords& get_normal_texture_coordinates(void) const noexcept;
        const CharacterOffset& get_offset(void) const noexcept;
        const CharacterSizeInfo& size(void) const noexcept;
        uint16_t get_image_pos_x(void) const noexcept;
        uint16_t get_image_pos_y(void) const noexcept;
        unsigned char get_id(void) const noexcept;
        void set_offset(const CharacterOffset& offset) noexcept;
        void calc(const ImageInfo& img_info);

        Character& operator= (Character&& obj);
        Character& operator= (const Character& obj);
    };

    /******************************************************************************************************************************************
    *   Class: string
    *
    *   Description: Gnerates mesh-data and model-matrix for the graphical impelmentation of the string.
    *                It contains a variable that indicates if the model-matrix or the mesh-data has changed.
    *                This might be useful for asynchronous mesh and model handling for strings.
    *
    *   Info: String-class does not draw anything to the graphic scene anymore.
    *
    ******************************************************************************************************************************************/

    class string
    {
    private:
        glm::mat4 model_matrix;

        std::string text;
        std::vector<float> colors;

        Font* used_font;
        float font_size, col_space, row_space, tab_size;

        float fixx, fixy, fixz;
        float x, y, z;
        float rotx, roty, rotz;
        float scalex, scaley, scalez;

        float* mesh_buff;

        std::atomic_bool __mesh_changed, __model_changed;

    public:
        string(void);
        string(const std::string& str, const Font& font, float font_size, const std::vector<float>& colors = {});
        string(const char* str, const Font& font, float font_size, const std::vector<float>& colors = {});

        string(const string&);
        string& operator= (const string&);

        string(string&&);
        string& operator= (string&&);

        virtual ~string(void);

        // getter
        const Font& get_font(void) const noexcept                   {return *this->used_font;}
        void get_color(size_t i, float& r, float& g, float& b, float& a) const;
        const std::vector<float>& get_colors(void) const noexcept   {return this->colors;}
        float get_font_size(void) const noexcept                    {return this->font_size;}
        float get_col_space(void) const noexcept                    {return this->col_space;}
        float get_row_space(void) const noexcept                    {return this->row_space;}
        float get_tab_size(void) const noexcept                     {return this->tab_size;}
        float get_fix_pos_x(void) const noexcept                    {return this->fixx;}
        float get_fix_pos_y(void) const noexcept                    {return this->fixy;}
        float get_fix_pos_z(void) const noexcept                    {return this->fixz;}
        void get_fix_pos(float& x, float& y, float& z) const noexcept;
        float get_pos_x(void) const noexcept                        {return this->x;}
        float get_pos_y(void) const noexcept                        {return this->y;}
        float get_pos_z(void) const noexcept                        {return this->z;}
        void get_pos(float& x, float& y, float& z) const noexcept;
        float get_angle_x(void) const noexcept                      {return this->rotx;}
        float get_angle_y(void) const noexcept                      {return this->roty;}
        float get_angle_z(void) const noexcept                      {return this->rotz;}
        void get_angle(float& x, float& y, float& z) const noexcept;
        float get_scale_x(void) const noexcept                      {return this->scalex;}
        float get_scale_y(void) const noexcept                      {return this->scaley;}
        float get_scale_z(void) const noexcept                      {return this->scalez;}
        void get_scale(float& x, float& y, float& z) const noexcept;
        const std::string& std_str(void) const noexcept             {return this->text;}
        size_t length(void) const noexcept                          {return this->text.length();}
        size_t size(void) const noexcept                            {return this->text.size();}
        char& at(size_t i)                                          {return this->text.at(i);}
        char at(size_t i) const                                     {return this->text.at(i);}
        int find(char c) const noexcept;
        int find_color(float r, float g, float b, float a) const noexcept;
        int find_char_color(char c, float r, float g, float b, float a) const noexcept;

        void set_str(const std::string& str);
        void set_str(const char* str);
        void set_color(size_t i, float r, float g, float b, float a);
        void set_color_all(float r, float g, float b, float a) noexcept;
        void set_colors(const std::vector<float>& colors) noexcept;
        void set_font(const Font& font) noexcept            {this->used_font = const_cast<Font*>(&font);}
        void set_font_size(float font_size) noexcept        {this->font_size = font_size; this->__mesh_changed = true;}
        void set_col_space(float col_space) noexcept        {this->col_space = col_space; this->__mesh_changed = true;}
        void set_row_space(float row_space) noexcept        {this->row_space = row_space; this->__mesh_changed = true;}
        void set_tab_size(float tab_size) noexcept          {this->tab_size = tab_size;}
        void set_fix_pos_x(float fpos_x) noexcept           {this->fixx = fpos_x; this->__mesh_changed = true;}
        void set_fix_pos_y(float fpos_y) noexcept           {this->fixy = fpos_y; this->__mesh_changed = true;}
        void set_fix_pos_z(float fpos_z) noexcept           {this->fixz = fpos_z; this->__mesh_changed = true;}
        void set_fix_pos(float x, float y, float z) noexcept;
        void set_pos_x(float pos_x) noexcept                {this->x = pos_x; this->__model_changed = true;}
        void set_pos_y(float pos_y) noexcept                {this->y = pos_y; this->__model_changed = true;}
        void set_pos_z(float pos_z) noexcept                {this->z = pos_z; this->__model_changed = true;}
        void set_pos(float x, float y, float z) noexcept;
        void rotate_x(float angle_x) noexcept               {this->rotx = angle_x; this->__model_changed = true;}
        void rotate_y(float angle_y) noexcept               {this->roty = angle_y; this->__model_changed = true;}
        void rotate_z(float angle_z) noexcept               {this->rotz = angle_z; this->__model_changed = true;}
        void rotate(float angle_x, float angle_y, float angle_z) noexcept;
        void scale_x(float scale_x) noexcept                {this->scalex = scale_x; this->__model_changed = true;}
        void scale_y(float scale_y) noexcept                {this->scalex = scale_y; this->__model_changed = true;}
        void scale_z(float scale_z) noexcept                {this->scalex = scale_z; this->__model_changed = true;}
        void scale(float x, float y, float z) noexcept;

        void push_back(char c);
        void push_back(char c, float r, float g, float b, float a);
        void push_front(char c);
        void push_front(char c, float r, float g, float b, float a);
        char pop_back(void) noexcept;
        char pop_front(void) noexcept;
        void insert(size_t i, char c);
        void insert(size_t i, char c, float r, float g, float b, float a);
        void fill(size_t ibegin, size_t iend, char new_char);
        void fill_color(size_t ibegin, size_t iend, float r, float g, float b, float a);
        void erase(size_t ibegin, size_t iend);
        void erase(size_t i);
        void erase_color(size_t ibegin, size_t iend) noexcept;
        void erase_color(size_t i) noexcept;
        void clear(void) noexcept;
        void clear_color(void) noexcept;
        void clear_model(void) noexcept;
        void clear_mesh(void) noexcept;

        const glm::mat4& calc_model(std::string priority_string) noexcept;
        const glm::mat4& model(void) const noexcept {return this->model_matrix;}
        bool model_changed(void) const noexcept     {return this->__model_changed;}

        float* generate_mesh(void);
        float* mesh(void) const noexcept            {return this->mesh_buff;}
        size_t mesh_size(void) const noexcept;
        size_t mesh_stride(void) const noexcept;
        bool mesh_changed(void) const noexcept      {return this->__mesh_changed;}

        size_t vertex_count(void) const noexcept;
        size_t buff_size(void) const noexcept       {return this->mesh_size() * sizeof(float);}

        virtual bool operator== (const string& str) const noexcept;
        virtual bool operator!= (const string& str) const noexcept  {return !(*this == str);}
        virtual bool operator<  (const string& str) const noexcept  {return this->text < str.text;}
        virtual bool operator>  (const string& str) const noexcept  {return this->text > str.text;}
        virtual bool operator<= (const string& str) const noexcept  {return this->text <= str.text;}
        virtual bool operator>= (const string& str) const noexcept  {return this->text >= str.text;}
        virtual char& operator[] (size_t i)                         {return this->text[i];}
        virtual char operator[] (size_t i) const noexcept           {return this->text[i];}

        inline static int get_vertex_offset_ptr(void) noexcept      {return 0;}
        inline static int get_color_offset_ptr(void) noexcept       {return 3*sizeof(float);}
        inline static int get_tex_offset_ptr(void) noexcept         {return 3*sizeof(float) + 4*sizeof(float);}
    };
};

#endif // GLSTR_H_INCLUDED
