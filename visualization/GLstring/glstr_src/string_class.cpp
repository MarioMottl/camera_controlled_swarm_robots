#include "../glstr.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

using namespace gl;

string::string(void)
{
    this->model_matrix = glm::mat4(1.0f);
    this->used_font = nullptr;
    this->fontsize = 0.0f;
    this->colspace = 0.0f;
    this->rowspace = 0.0f;
    this->tabsize = 0.0f;
    this->set_fix_pos(0.0f, 0.0f, 0.0f);
    this->set_pos(0.0f, 0.0f, 0.0f);
    this->rotate(0.0f, 0.0f, 0.0f);
    this->scale(1.0f, 1.0f, 1.0f);
    this->mesh_buff = nullptr;
}

string::string(const std::string& str, const Font& font, float font_size, const std::vector<float>& colors)
{
    this->model_matrix = glm::mat4(1.0f);
    this->set_str(str);
    this->set_colors(colors);
    this->used_font = const_cast<Font*>(&font);
    this->fontsize = font_size;
    this->colspace = 0.0f;
    this->rowspace = 0.0f;
    this->tabsize = 0.0f;
    this->set_fix_pos(0.0f, 0.0f, 0.0f);
    this->set_pos(0.0f, 0.0f, 0.0f);
    this->rotate(0.0f, 0.0f, 0.0f);
    this->scale(1.0f, 1.0f, 1.0f);
    this->mesh_buff = nullptr;
    this->__mesh_changed = true;
    this->__model_changed = true;
}

string::string(const char* str, const Font& font, float font_size, const std::vector<float>& colors)
{
    this->model_matrix = glm::mat4(1.0f);
    this->set_str(str);
    this->set_colors(colors);
    this->used_font = const_cast<Font*>(&font);
    this->fontsize = font_size;
    this->colspace = 0.0f;
    this->rowspace = 0.0f;
    this->tabsize = 0.0f;
    this->set_fix_pos(0.0f, 0.0f, 0.0f);
    this->set_pos(0.0f, 0.0f, 0.0f);
    this->rotate(0.0f, 0.0f, 0.0f);
    this->scale(1.0f, 1.0f, 1.0f);
    this->mesh_buff = nullptr;
    this->__mesh_changed = true;
    this->__model_changed = true;
}

string::string(const string& str)
{
    *this = str;
}

string::string(string&& str)
{
    *this = std::move(str);
}

string::~string(void)
{
    this->clear_mesh();
}

string& string::operator= (const string& str)
{
    this->model_matrix = str.model_matrix;
    this->text = str.text;
    this->used_font = str.used_font;
    this->fontsize = str.fontsize;
    this->colspace = str.colspace;
    this->rowspace = str.rowspace;
    this->tabsize = str.tabsize;
    this->colors = str.colors;
    this->x = str.x;
    this->y = str.y;
    this->z = str.z;
    this->rotx = str.rotx;
    this->roty = str.roty;
    this->rotz = str.rotz;
    this->scalex = str.scalex;
    this->scaley = str.scaley;
    this->scalez = str.scalez;
    this->fixx = str.fixx;
    this->fixy = str.fixy;
    this->fixz = str.fixz;
    return *this;
}

string& string::operator= (string&& str)
{
    this->model_matrix = str.model_matrix;
    str.model_matrix = glm::mat4(1.0f);

    this->text = std::move(str.text);
    this->colors = std::move(str.colors);

    this->used_font = str.used_font;
    str.used_font = nullptr;

    this->fontsize = str.fontsize;
    str.fontsize = 0.0f;

    this->colspace = str.colspace;
    str.colspace = 0.0f;

    this->rowspace = str.rowspace;
    str.rowspace = 0.0f;

    this->tabsize = str.tabsize;
    str.tabsize = 0.0f;

    this->x = str.x;
    str.x = 0.0f;

    this->y = str.y;
    str.y = 0.0f;

    this->z = str.z;
    str.z = 0.0f;

    this->rotx = str.rotx;
    str.rotx = 0.0f;

    this->roty = str.roty;
    str.roty = 0.0f;

    this->rotz = str.rotz;
    str.rotz = 0.0f;

    this->scalex = str.scalex;
    str.scalex = 0.0f;

    this->scaley = str.scaley;
    str.scalex = 0.0f;

    this->scalez = str.scalez;
    str.scalex = 0.0f;

    this->fixx = str.fixx;
    str.fixx = 0.0f;

    this->fixy = str.fixy;
    str.fixy = 0.0f;

    this->fixz = str.fixz;
    str.fixz = 0.0f;

    if(this->mesh_buff != nullptr)
        this->clear_mesh();
    this->mesh_buff = str.mesh_buff;
    str.mesh_buff = nullptr;

    return *this;
}

void string::get_color(size_t i, float& r, float& g, float& b, float& a) const
{
    r = this->colors.at(i+0);
    g = this->colors.at(i+1);
    b = this->colors.at(i+2);
    a = this->colors.at(i+3);
}

void string::fix_pos(float& x, float& y, float& z) const noexcept
{
    x = this->fixx;
    y = this->fixy;
    z = this->fixz;
}

void string::pos(float& x, float& y, float& z) const noexcept
{
    x = this->x;
    y = this->y;
    z = this->z;
}

void string::angle(float& x, float& y, float& z) const noexcept
{
    x = this->rotx;
    y = this->roty;
    z = this->rotz;
}

void string::get_scale(float& x, float& y, float& z) const noexcept
{
    x = this->scalex;
    y = this->scaley;
    z = this->scalez;
}

int string::find(char c) const noexcept
{
    for(size_t i=0; i<text.size(); i++)
    {
        if(this->text.at(i) == c)
            return i;
    }
    return -1;
}

int string::find_color(float r, float g, float b, float a) const noexcept
{
    for(size_t i=0; i<text.size(); i++)
    {
        if(this->colors.at(i+0) == r &&
           this->colors.at(i+1) == g &&
           this->colors.at(i+2) == b &&
           this->colors.at(i+3) == a)
        {
            return i;
        }
    }
    return -1;
}

int string::find_char_color(char c, float r, float g, float b, float a) const noexcept
{
    for(size_t i=0; i<text.size(); i++)
    {
        if(this->colors.at(i+0) == r &&
           this->colors.at(i+1) == g &&
           this->colors.at(i+2) == b &&
           this->colors.at(i+3) == a &&
           this->text.at(i) == c)
        {
            return i;
        }
    }
    return -1;
}

void string::set_str(const std::string& str)
{
    this->text = str;
    this->colors.resize(str.size() * 4, 1.0f);
    this->__mesh_changed = true;
}

void string::set_str(const char* str)
{
    this->text = str;
    this->colors.resize(this->text.size() * 4, 1.0f);
    this->__mesh_changed = true;
}

void string::set_color(size_t i, float r, float g, float b, float a)
{
    this->colors.at(i*4+0) = r;
    this->colors.at(i*4+1) = g;
    this->colors.at(i*4+2) = b;
    this->colors.at(i*4+3) = a;
    this->__mesh_changed = true;
}

void string::set_color_all(float r, float g, float b, float a) noexcept
{
    for(size_t i=0; i<text.size(); i++)
    {
        this->colors[i*4+0] = r;
        this->colors[i*4+1] = g;
        this->colors[i*4+2] = b;
        this->colors[i*4+3] = a;
    }
    this->__mesh_changed = true;
}

void string::set_colors(const std::vector<float>& colors) noexcept
{
    for(size_t i=0; i < this->text.size(); i++)
    {
        if(i*4 < colors.size())
        {
            for(size_t j=0; j<4; j++)
                this->colors[i*4+j] = colors[i*4+j];
        }
        else
        {
            for(size_t j=0; j<4; j++)
                this->colors[i*4+j] = 1.0f;   // default color = white with full opacity
        }
    }
    this->__mesh_changed = true;
}

void string::set_fix_pos(float x, float y, float z) noexcept
{
    this->fixx = x;
    this->fixy = y;
    this->fixz = z;
    this->__mesh_changed = true;
}

void string::set_pos(float x, float y, float z) noexcept
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->__model_changed = true;
}

void string::rotate(float angle_x, float angle_y, float angle_z) noexcept
{
    this->rotx = angle_x;
    this->roty = angle_y;
    this->rotz = angle_z;
    this->__model_changed = true;
}

void string::scale(float x, float y, float z) noexcept
{
    this->scalex = x;
    this->scaley = y;
    this->scalez = z;
    this->__model_changed = true;
}

void string::push_back(char c)
{
    this->text.push_back(c);
    this->colors.push_back(1.0f);
    this->colors.push_back(1.0f);
    this->colors.push_back(1.0f);
    this->colors.push_back(1.0f);
    this->__mesh_changed = true;
}

void string::push_back(char c, float r, float g, float b, float a)
{
    this->text.push_back(c);
    this->colors.push_back(r);
    this->colors.push_back(g);
    this->colors.push_back(b);
    this->colors.push_back(a);
    this->__mesh_changed = true;
}

void string::push_front(char c)
{
    this->text.insert(this->text.begin(), c);
    this->colors.insert(this->colors.begin(), 1.0f);
    this->colors.insert(this->colors.begin(), 1.0f);
    this->colors.insert(this->colors.begin(), 1.0f);
    this->colors.insert(this->colors.begin(), 1.0f);
    this->__mesh_changed = true;
}

void string::push_front(char c, float r, float g, float b, float a)
{
    this->text.insert(this->text.begin(), c);
    this->colors.insert(this->colors.begin(), a);
    this->colors.insert(this->colors.begin(), b);
    this->colors.insert(this->colors.begin(), g);
    this->colors.insert(this->colors.begin(), r);
    this->__mesh_changed = true;
}

char string::pop_back(void) noexcept
{
    char c = 0;
    if(this->text.size() > 0)
    {
        c = this->text.at(this->text.size() - 1);
        this->text.pop_back();
        for(int i=0; i<4; i++)
            this->colors.pop_back();
        this->__mesh_changed = true;
    }
    return c;
}

char string::pop_front(void) noexcept
{
    char c = 0;
    if(this->text.size() > 0)
    {
        c = this->text.at(this->text.size() - 1);
        this->text.erase(this->text.begin());
        for(int i=0; i<4; i++)
            this->colors.erase(this->colors.begin());
        this->__mesh_changed = true;
    }
    return c;
}

void string::insert(size_t i, char c)
{
    this->text.insert(this->text.begin() + i, c);
    this->colors.insert(this->colors.begin() + i*4+0, 1.0f);
    this->colors.insert(this->colors.begin() + i*4+1, 1.0f);
    this->colors.insert(this->colors.begin() + i*4+2, 1.0f);
    this->colors.insert(this->colors.begin() + i*4+3, 1.0f);
    this->__mesh_changed = true;
}

void string::insert(size_t i, char c, float r, float g, float b, float a)
{
    this->text.insert(this->text.begin() + i, c);
    this->colors.insert(this->colors.begin() + i*4+0, r);
    this->colors.insert(this->colors.begin() + i*4+1, g);
    this->colors.insert(this->colors.begin() + i*4+2, b);
    this->colors.insert(this->colors.begin() + i*4+3, a);
    this->__mesh_changed = true;
}

void string::fill(size_t ibegin, size_t iend, char new_char)
{
    for(size_t i = ibegin; i<=iend; i++)
        this->text.at(i) = new_char;
    this->__mesh_changed = true;
}

void string::fill_color(size_t ibegin, size_t iend, float r, float g, float b, float a)
{
    for(size_t i = ibegin; i<=iend; i++)
    {
        this->colors.at(i*4+0) = r;
        this->colors.at(i*4+1) = g;
        this->colors.at(i*4+2) = b;
        this->colors.at(i*4+3) = a;
    }
    this->__mesh_changed = true;
}

void string::erase(size_t ibegin, size_t iend)
{
    this->text.erase(this->text.begin() + ibegin, this->text.begin() + iend + 1);
    this->colors.erase(this->colors.begin() + ibegin*4, this->colors.begin() + (iend+1)*4);
    this->__mesh_changed = true;
}

void string::erase(size_t i)
{
    this->erase(i, i);
}

void string::erase_color(size_t ibegin, size_t iend) noexcept
{
    for(size_t i=ibegin*4; i<(iend+1)*4; i++)
        this->colors.at(i) = 1.0f;
    this->__mesh_changed = true;
}

void string::erase_color(size_t i) noexcept
{
    this->erase_color(i, i);
}

void string::clear(void) noexcept
{
    this->text.clear();
    this->colors.clear();
    this->__mesh_changed = true;
}

void string::clear_color(void) noexcept
{
    for(size_t i=0; i<this->colors.size(); i++)
        this->colors.at(i) = 1.0f;
    this->__mesh_changed = true;
}

void string::clear_model(void) noexcept
{
    this->model_matrix = glm::mat4(1.0f);
}

void string::clear_mesh(void) noexcept
{
    delete(this->mesh_buff);
    this->mesh_buff = nullptr;
}

const glm::mat4& string::calc_model(std::string priority_string) noexcept
{
    priority_string.resize((priority_string.size() > 3) ? 3 : priority_string.size());

    for(int i=priority_string.size()-1; i>=0; i--)
    {
        if(priority_string.at(i) == 't' || priority_string.at(i) == 'T')
        {
            this->model_matrix = glm::translate(this->model_matrix, glm::vec3(this->x, this->y, this->z));
        }
        else if(priority_string.at(i) == 'r' || priority_string.at(i) == 'R')
        {
            this->model_matrix = glm::rotate(this->model_matrix, this->rotx, glm::vec3(1.0f, 0.0f, 0.0f));
            this->model_matrix = glm::rotate(this->model_matrix, this->roty, glm::vec3(0.0f, 1.0f, 0.0f));
            this->model_matrix = glm::rotate(this->model_matrix, this->rotz, glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else if(priority_string.at(i) == 's' || priority_string.at(i) == 'S')
        {
            this->model_matrix = glm::scale(this->model_matrix, glm::vec3(this->scalex, this->scaley, this->scalez));
        }
    }
    return this->model_matrix;
}

float* string::generate_mesh(void)
{
    if(this->used_font == nullptr)
        return nullptr;

    this->clear_mesh();
    this->mesh_buff = new float[this->mesh_size()];
    float x_offset = 0.0f, y_offset = 0.0f;

    for(size_t i=0; i<text.size(); i++)
    {
        if(text[i] == '\n')
        {
            y_offset -= this->fontsize + this->rowspace;   // '\n'
        }
        else if(text[i] == '\r')
        {
            x_offset = 0.0f;
        }
        else if(text[i] == '\t')
        {
            x_offset = x_offset + this->tabsize - fmod(x_offset, this->tabsize);
        }
        else
        {
            const float char_ratio = this->used_font->at(text[i]).size().char_size_x / (float)this->used_font->at(text[i]).size().char_max_size_y;

            // calculate vertices
            const float vertices[12] = {
                x_offset + this->fontsize * char_ratio + this->fixx,   // x1
                y_offset + this->fixy,                                  // y1
                this->fixz,                                             // z1

                x_offset + this->fixx,                                  // x2
                y_offset + this->fixy,                                  // y2
                this->fixz,                                             // z2

                x_offset + this->fixx,                                  // x3
                y_offset - this->fontsize + this->fixy,                // y3
                this->fixz,                                             // z3

                x_offset + this->fontsize * char_ratio + this->fixx,   // x4
                y_offset - this->fontsize + this->fixy,                // y4
                this->fixz                                              // z4
            };

            x_offset += this->fontsize * char_ratio + this->colspace;

            for(int vertex = 0; vertex < 4; vertex++)
            {
                for(int v = 0; v < 3; v++)
                {
                    this->mesh_buff[i*__char_vertex_count*__component_count + vertex*__component_count + v+0] = vertices[vertex*3 + v];    // 0,1,2
                }
                for(int c = 0; c < 4; c++)
                {
                    this->mesh_buff[i*__char_vertex_count*__component_count + vertex*__component_count + c+3] = this->colors.at(i*4 + c); // 3,4,5,6
                }

                const float* const tex_coords_x = (float*)&this->used_font->at(text[i]).texture_coordinates();
                const float* const tex_coords_y = (float*)&this->used_font->at(text[i]).normal_texture_coordinates();
                this->mesh_buff[i*__char_vertex_count*__component_count + vertex*__component_count + 7] = tex_coords_x[vertex*2 + 0];
                this->mesh_buff[i*__char_vertex_count*__component_count + vertex*__component_count + 8] = tex_coords_y[vertex*2 + 1];
            }
        }
    }
    this->__mesh_changed = false;
    return this->mesh_buff;
}

size_t string::mesh_size(void) const noexcept
{
    // text.size() characters / 4 vertices each character / 9 components (XYZ coordinate, RGBA color, ST texture coordinate) each vertex
    return this->text.size() * __char_vertex_count * __component_count;
}

size_t string::mesh_stride(void) const noexcept
{
    return sizeof(float) * __component_count;
}

size_t string::vertex_count(void) const noexcept
{
    return this->text.size() * __char_vertex_count;
}

bool string::operator== (const string& str) const noexcept  
{
    return (this->text == str.text && 
            this->colors == str.colors &&
            this->used_font == str.used_font &&
            this->fontsize == str.fontsize &&
            this->colspace == str.colspace &&
            this->rowspace == str.rowspace &&
            this->tabsize == str.tabsize &&
            this->x == str.x &&
            this->y == str.y &&
            this->z == str.z &&
            this->fixx == str.fixx &&
            this->fixy == str.fixy &&
            this->fixz == str.fixz &&
            this->rotx == str.rotx &&
            this->roty == str.roty &&
            this->rotz == str.rotz &&
            this->scalex == str.scalex &&
            this->scaley == str.scaley &&
            this->scalez == str.scalez);
}