#include "../glstr.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace gl;

Font::Font(uint16_t num_x_chars, uint16_t num_y_chars)
{
    this->img_info.height = 0;
    this->img_info.width = 0;
    this->set_num_chars(num_x_chars, num_y_chars);

    // generate texture object for this font
    glGenTextures(1, &this->font_texture_id);
}

Font::Font(const Font& obj) {*this = obj;}
Font::Font(Font&& obj) {*this = std::move(obj);}

Font::~Font(void)
{
    glDeleteTextures(1, &this->font_texture_id);
}

Character& Font::at(unsigned char c)                    {return this->chars.at(c);}
const Character& Font::at(unsigned char c) const        {return this->chars.at(c);}
uint32_t Font::texture_id(void) const noexcept          {return this->font_texture_id;}
const ImageInfo& Font::info(void) const noexcept        {return this->img_info;}
uint16_t Font::num_chars(void) const noexcept           {return this->chars.size();}

void Font::set_num_chars(uint16_t num_x_chars, uint16_t num_y_chars) noexcept
{
    this->img_info.x_chars = num_x_chars;    // set character sizes
    this->img_info.y_chars = num_y_chars;

    this->chars.resize(num_x_chars * num_y_chars);  // resize character vector
}

FontError Font::load(const std::string& image_path, bool linear_filter, const std::vector<CharacterOffset>& offsets)
{
    if(this->num_chars() == 0)  // if x_chars or y_chars is 0 ... num of characters is 0
        return FontError::FONT_ERROR_INVALID_NUM_CHARS;

    int width, height;
    uint8_t* img_data = stbi_load(image_path.c_str(), &width, &height, nullptr, 4); // load image file

    if(img_data == nullptr)     // if loading has failed return with error
        return FontError::FONT_ERROR_INVALID_PATH;
    if(width == 0 || height == 0)
        return FontError::FONT_ERROR_INVALID_IMAGE_SIZE;

    this->img_info.width = width;      // set image with
    this->img_info.height = height;    // set image height

    // bind texture object
    glBindTexture(GL_TEXTURE_2D, this->font_texture_id);
    // set texture parameter for binded texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (linear_filter) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (linear_filter) ? GL_LINEAR : GL_NEAREST);
    // set texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->img_info.width, this->img_info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
    glGenerateMipmap(GL_TEXTURE_2D);    // generate mipmaps (mipmap levels) for binded texture
    glBindTexture(GL_TEXTURE_2D, 0);    // unbind texture
    stbi_image_free(img_data);          // free allocated memory for image data

    // initialize all characters of image file
    for(size_t i=0; i < chars.size(); i++)
    {
        this->chars.at(i).set_id(i);

        if(offsets.size() < this->chars.size())
            this->chars.at(i).set_offset({0,0,0,0});
        else
            this->chars.at(i).set_offset(offsets.at(i));

        this->chars.at(i).calc(img_info);
    }
    return FontError::FONT_ERROR_NONE;
}

void Font::bind(void) const
{
    glBindTexture(GL_TEXTURE_2D, this->font_texture_id);
}

void Font::unbind(void) const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Font& Font::operator= (const Font& obj)
{
    this->font_texture_id = obj.font_texture_id;
    this->img_info = obj.img_info;
    this->chars = obj.chars;
    return *this;
}

Font& Font::operator= (Font&& obj)
{
    this->font_texture_id = obj.font_texture_id;
    obj.font_texture_id = 0;

    this->img_info = obj.img_info;
    obj.img_info = {0,0,0,0};

    this->chars = std::move(obj.chars);
    return *this;
}
