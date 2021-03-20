#include "../glstr.h"

using namespace gl;

Character::Character(const CharacterOffset& offset, unsigned char id)
{
    this->__offset = offset;
    this->__id = id;
    this->pos_on_imgX = 0;
    this->pos_on_imgY = 0;
    this->size_info = {0,0,0,0};
    this->tex_coords = {0.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f};
    this->normal_tex_coords = {0.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f};
}

Character::Character(void) : Character({0,0,0,0}, 0) {}
Character::Character(const Character& obj) {*this = obj;}
Character::Character(Character&& obj) {*this = std::move(obj);}
Character::~Character(void) {/* dtor */}

const CharacterTexCoords& Character::texture_coordinates(void) const noexcept           {return this->tex_coords;}
const CharacterTexCoords& Character::normal_texture_coordinates(void) const noexcept    {return this->normal_tex_coords;}

const CharacterOffset& Character::offset(void) const noexcept       {return this->__offset;}
const CharacterSizeInfo& Character::size(void) const noexcept       {return this->size_info;}
uint16_t Character::image_pos_x(void) const noexcept                {return this->pos_on_imgX;}
uint16_t Character::image_pos_y(void) const noexcept                {return this->pos_on_imgY;}
unsigned char Character::id(void) const noexcept                    {return this->__id;}
void Character::set_offset(const CharacterOffset& offset) noexcept  {this->__offset = offset;}
void Character::set_id(unsigned char id) noexcept                   {this->__id = id;}

void Character::calc(const ImageInfo& font_img_info)
{
    // calculate width and height of character, size in PIXELS
    this->size_info.char_max_size_x = font_img_info.width / (float)font_img_info.x_chars;     // width without offsets
    this->size_info.char_max_size_y = font_img_info.height / (float)font_img_info.y_chars;    // height without offsets
    this->size_info.char_size_x = this->size_info.char_max_size_x - this->__offset.positive_x - this->__offset.negative_x;  // width with offsets
    this->size_info.char_size_y = this->size_info.char_max_size_y - this->__offset.positive_y - this->__offset.negative_y;  // height with offsets

    // calculate position of character on image
    this->pos_on_imgX = font_img_info.width * (__id % font_img_info.x_chars) / font_img_info.x_chars;
    this->pos_on_imgY = font_img_info.height * (__id / font_img_info.y_chars) / font_img_info.y_chars;

    // calculate top right texture coordinate
    this->tex_coords.top_right_x = (this->pos_on_imgX + this->size_info.char_max_size_x - this->__offset.negative_x) / static_cast<float>(font_img_info.width);
    this->tex_coords.top_right_y = (this->pos_on_imgY + this->__offset.positive_y) / static_cast<float>(font_img_info.height);
    this->normal_tex_coords.top_right_x = (this->pos_on_imgX + this->size_info.char_max_size_x) / static_cast<float>(font_img_info.width);
    this->normal_tex_coords.top_right_y = this->pos_on_imgY / static_cast<float>(font_img_info.height);

    // calculate top left texture coordinate
    this->tex_coords.top_left_x = (this->pos_on_imgX + this->__offset.positive_x) / static_cast<float>(font_img_info.width);
    this->tex_coords.top_left_y = (this->pos_on_imgY + this->__offset.positive_y) / static_cast<float>(font_img_info.height);
    this->normal_tex_coords.top_left_x = this->pos_on_imgX / static_cast<float>(font_img_info.width);
    this->normal_tex_coords.top_left_y = this->pos_on_imgY / static_cast<float>(font_img_info.height);

    // calculate bottom left texture coordinate
    this->tex_coords.bottom_left_x = (this->pos_on_imgX + this->__offset.positive_x) / static_cast<float>(font_img_info.width);
    this->tex_coords.bottom_left_y = (this->pos_on_imgY + this->size_info.char_max_size_y - this->__offset.negative_y) / static_cast<float>(font_img_info.height);
    this->normal_tex_coords.bottom_left_x = this->pos_on_imgX / static_cast<float>(font_img_info.width);
    this->normal_tex_coords.bottom_left_y = (this->pos_on_imgY + this->size_info.char_max_size_y) / static_cast<float>(font_img_info.height);

    // calculate bottom right texture coordinate
    this->tex_coords.bottom_right_x = (this->pos_on_imgX + this->size_info.char_max_size_x - this->__offset.negative_x) / static_cast<float>(font_img_info.width);
    this->tex_coords.bottom_right_y = (this->pos_on_imgY + this->size_info.char_max_size_y - this->__offset.negative_y) / static_cast<float>(font_img_info.height);
    this->normal_tex_coords.bottom_right_x = (this->pos_on_imgX + this->size_info.char_max_size_x) / static_cast<float>(font_img_info.width);
    this->normal_tex_coords.bottom_right_y = (this->pos_on_imgY + this->size_info.char_max_size_y) / static_cast<float>(font_img_info.height);
}

Character& Character::operator= (const Character& obj)
{
    this->__offset = obj.__offset;
    this->__id = obj.__id;
    this->pos_on_imgX = obj.pos_on_imgX;
    this->pos_on_imgY = obj.pos_on_imgY;
    this->size_info = obj.size_info;
    this->tex_coords = obj.tex_coords;
    this->normal_tex_coords = obj.normal_tex_coords;
    return *this;
}

Character& Character::operator= (Character&& obj)
{
    this->__offset = obj.__offset;
    obj.__offset = {0,0,0,0};

    this->__id = obj.__id;
    obj.__id = 0;

    this->pos_on_imgX = obj.pos_on_imgX;
    obj.pos_on_imgX = 0;

    this->pos_on_imgY = obj.pos_on_imgY;
    obj.pos_on_imgY = 0;

    this->size_info = obj.size_info;
    obj.size_info = {0,0,0,0};

    this->tex_coords = obj.tex_coords;
    obj.tex_coords = {0,0, 0,0, 0,0, 0,0};

    this->normal_tex_coords = obj.normal_tex_coords;
    obj.normal_tex_coords = {0,0, 0,0, 0,0, 0,0};
    return *this;
}
