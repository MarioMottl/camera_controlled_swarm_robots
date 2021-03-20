/******************************************************************************************************************************************
* Title:        glstr
* Author:       Michael Reim
* Date:         14.08.2020
* Description:
*   Library to implement strings and texts on an OpenGL graphic scene.
*
* @version release 2.1.0
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
        uint16_t hepositive_xight{0};
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
        FONT_ERROR_NONE,
        FONT_ERROR_INVALID_PATH,
        FONT_ERROR_INVALID_IMAGE_SIZE,
        FONT_ERROR_INVALID_NUM_CHARS
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
        bool linear_filtering;

    public:
        Font(uint16_t num_x_chars = 16, uint16_t num_y_chars = 16);
        Font(const Font& obj);
        Font(Font&& obj);
        virtual ~Font(void);

        const Character& at(unsigned char c) const;
        Character& at(unsigned char c);
        uint32_t texture_id(void) const noexcept;
        const ImageInfo& info(void) const noexcept;
        uint16_t num_chars(void) const noexcept;
        void set_num_chars(uint16_t num_x_chars, uint16_t num_y_chars) noexcept;
        void set_linear_filtering(bool filtering) noexcept;
        bool get_linear_filtering(void) const noexcept;

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
        unsigned char __id;
        uint16_t pos_on_imgX, pos_on_imgY;
        CharacterOffset __offset;
        CharacterSizeInfo size_info;
        CharacterTexCoords tex_coords;
        CharacterTexCoords normal_tex_coords;

    public:
        Character(void);
        Character(const CharacterOffset& offset, unsigned char id);
        Character(const Character& obj);
        Character(Character&& obj);
        virtual ~Character(void);

        unsigned char id(void) const noexcept;
        void set_id(unsigned char id) noexcept;
        const CharacterTexCoords& texture_coordinates(void) const noexcept;
        const CharacterTexCoords& normal_texture_coordinates(void) const noexcept;
        const CharacterOffset& offset(void) const noexcept;
        void set_offset(const CharacterOffset& offset) noexcept;
        const CharacterSizeInfo& size(void) const noexcept;
        uint16_t image_pos_x(void) const noexcept;
        uint16_t image_pos_y(void) const noexcept;
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
        static constexpr size_t __char_vertex_count = 4;
        static constexpr size_t __vertex_component_count = 3;
        static constexpr size_t __color_component_count = 4;
        static constexpr size_t __tex_component_count = 2;
        static constexpr size_t __component_count = __vertex_component_count + __color_component_count + __tex_component_count;

        glm::mat4 model_matrix;

        std::string text;
        std::vector<float> colors;

        Font* used_font;
        float fontsize, colspace, rowspace, tabsize;

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
        float font_size(void) const noexcept                        {return this->fontsize;}
        float col_space(void) const noexcept                        {return this->colspace;}
        float row_space(void) const noexcept                        {return this->rowspace;}
        float tab_size(void) const noexcept                         {return this->tabsize;}
        float fix_pos_x(void) const noexcept                        {return this->fixx;}
        float fix_pos_y(void) const noexcept                        {return this->fixy;}
        float fix_pos_z(void) const noexcept                        {return this->fixz;}
        void fix_pos(float& x, float& y, float& z) const noexcept;
        float pos_x(void) const noexcept                            {return this->x;}
        float pos_y(void) const noexcept                            {return this->y;}
        float pos_z(void) const noexcept                            {return this->z;}
        void pos(float& x, float& y, float& z) const noexcept;  
        float angle_x(void) const noexcept                          {return this->rotx;}
        float angle_y(void) const noexcept                          {return this->roty;}
        float angle_z(void) const noexcept                          {return this->rotz;}
        void angle(float& x, float& y, float& z) const noexcept;
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
        void set_font_size(float font_size) noexcept        {this->fontsize = font_size; this->__mesh_changed = true;}
        void set_col_space(float col_space) noexcept        {this->colspace = col_space; this->__mesh_changed = true;}
        void set_row_space(float row_space) noexcept        {this->rowspace = row_space; this->__mesh_changed = true;}
        void set_tab_size(float tab_size) noexcept          {this->tabsize = tab_size;}
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

        bool operator== (const string& str) const noexcept;
        bool operator!= (const string& str) const noexcept  {return !(*this == str);}
        bool operator<  (const string& str) const noexcept  {return this->text < str.text;}
        bool operator>  (const string& str) const noexcept  {return this->text > str.text;}
        bool operator<= (const string& str) const noexcept  {return this->text <= str.text;}
        bool operator>= (const string& str) const noexcept  {return this->text >= str.text;}
        char& operator[] (size_t i)                         {return this->text[i];}
        char operator[] (size_t i) const noexcept           {return this->text[i];}

        inline static int vertex_offset(void) noexcept                  {return 0;}
        inline static int color_offset(void) noexcept                   {return __vertex_component_count * sizeof(float);}
        inline static int tex_offset(void) noexcept                     {return (__vertex_component_count + __color_component_count) * sizeof(float);}
    	inline static size_t vertex_component_count(void) noexcept      {return __vertex_component_count;}
    	inline static size_t color_component_count(void) noexcept       {return __color_component_count;}
    	inline static size_t tex_component_count(void) noexcept         {return __tex_component_count;}
    	inline static size_t component_count(void) noexcept             {return __component_count;}
    	inline static size_t char_vertex_count(void) noexcept           {return __char_vertex_count;}
    };
};

#endif // GLSTR_H_INCLUDED
