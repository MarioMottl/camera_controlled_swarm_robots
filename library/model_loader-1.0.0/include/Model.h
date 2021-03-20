#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

/******************************************************************************************************************************************
* Title:        ModelLoader
* Author:       Michael Reim
* Date:         16.10.2020
* Description:
*   Library for easy model loading for OpenGL.
*   Can only read .obj and .mtl files.
*   The mesh data internally uses 32bit floatingpoint (float). For calculating the size in bytes you will need
*   to write class.method().size() * sizeof(float).
*
* @version release 1.0.0
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* If there are any bugs, contact me!
******************************************************************************************************************************************/

#include <string>
#include <vector>
#include <map>
#include <fstream>

namespace gl
{
    enum model_error_t
    {
        MODEL_ERROR_NONE,
        MODEL_FILE_NOT_FOUND,
        MATERIAL_FILE_NOT_FOUND
    };

    enum model_flag_t : int
    {
        __mesh_combined = 0x1,
        __ignore_vt = 0x2,
        __ignore_vn = 0x4,
        __swap_yz = 0x8,
        __invert_texture_s = 0x10,
        __invert_texture_t = 0x20
    };

    struct ModelColor3f
    {
        float r, g, b;
    };

    // model material data and mesh command always comes in a pair
    struct MeshMatierial
    {
        ModelColor3f ambient_color;
        ModelColor3f diffuse_color;
        ModelColor3f specular_color;
        float ambient_factor;
        float diffuse_factor;
        float specular_factor;
        size_t specular_highlight;  // specular exponent
        float opacity;

        // this information is lost if combined meshes are used
        std::string ambient_texture;
        std::string diffuse_texture;
        std::string specular_texture;
        std::string specular_highlight_map;
        std::string opacity_map;

        size_t vertex_offset;
        size_t vertex_count;
    };

    class Model
    {
    private:
        // material commands
        static constexpr char NEW_MATERIAL_COMMAND[]            = "newmtl";
        static constexpr char AMBIENT_LIGHT_COLOR_COMMAND[]     = "Ka";
        static constexpr char DIFFUSE_LIGHT_COLOR_COMMAND[]     = "Kd";
        static constexpr char SPECULAR_LIGHT_COLOR_COMMAND[]    = "Ks";
        static constexpr char SPECULAR_HILIGHT_COMMAND[]        = "Ns";
        static constexpr char OPACITY_COMMAND[]                 = "d";
        static constexpr char AMBIENT_TEXTURE_COMMAND[]         = "map_Ka";
        static constexpr char DIFFUSE_TEXTURE_COMMAND[]         = "map_Kd";
        static constexpr char SPECULAR_TEXTURE_COMMAND[]        = "map_Ks";
        static constexpr char SPECULAR_HIGHLIGHT_MAP_COMMAND[]  = "map_Ns";
        static constexpr char OPACITY_MAP_COMMAND[]             = "map_d";

        // object commands
        static constexpr char MTL_LIB_COMMAND[]                 = "mtllib";
        static constexpr char VERTEX_VOMMAND[]                  = "v";
        static constexpr char TEXTURE_COORD_COMMAND[]           = "vt";
        static constexpr char NORMAL_COMMAND[]                  = "vn";
        static constexpr char USE_MATERIAL_COMMAND[]            = "usemtl";
        static constexpr char FACE_COMMAND[]                    = "f";

        static constexpr size_t V_SIZE = 3;
        static constexpr size_t VT_SIZE = 2;
        static constexpr size_t VN_SIZE = 3;

        static constexpr size_t V_OFFSET = 0;
        static constexpr size_t VT_OFFSET = V_OFFSET + V_SIZE * sizeof(float);
        static constexpr size_t VN_OFFSET = V_OFFSET + VT_OFFSET + VT_SIZE * sizeof(float);

        static std::vector<std::string> __split(std::string& str, const char* delimeter);
        static std::vector<std::string> split_line(std::string& line)     {return Model::__split(line, " ");}
        static std::vector<std::string> split_face(std::string& face)     {return Model::__split(face, "/");}

        static float to_float(const std::string& str) noexcept;
        static size_t to_size(const std::string& str) noexcept;
        static bool is_comment(const std::string& str) noexcept;

        static int get_face_type(const std::string& str) noexcept;
        static std::string get_mtllib_path(std::ifstream& obj_file);

        std::vector<float> v_data;  // either vertex data or every data if combined mesh is enabled
        std::vector<float> vt_data; // vertex texture coords
        std::vector<float> vn_data; // normal vectors
        std::map<std::string, MeshMatierial> materials; // contains material information and the vertex range it is valid

        std::vector<float> v_temp, vt_temp, vn_temp;    // temporary buffers
        std::string last_material;
        size_t vertex_cntr, begin_vertex;

        void interpret_material_line(const std::vector<std::string>& mat_tokens);
        void interpret_object_line(const std::vector<std::string>& obj_tokens, int flags);

    public:
        static constexpr model_flag_t MESH_COMBINED = model_flag_t::__mesh_combined;
        static constexpr model_flag_t IGNORE_VT = model_flag_t::__ignore_vt;
        static constexpr model_flag_t IGNORE_VN = model_flag_t::__ignore_vn;
        static constexpr model_flag_t SWAP_YZ = model_flag_t::__swap_yz;
        static constexpr model_flag_t INVERT_S = model_flag_t::__invert_texture_s;
        static constexpr model_flag_t INVERT_T = model_flag_t::__invert_texture_t;

        static std::string strerror(model_error_t error);
        static size_t vertex_size(void) noexcept            {return V_SIZE;}
        static size_t vertex_offset(void) noexcept          {return V_OFFSET;}
        static size_t texture_coord_size(void) noexcept     {return VT_SIZE;}
        static size_t texture_coord_offset(void) noexcept   {return VT_OFFSET;}
        static size_t normal_size(void) noexcept            {return VN_SIZE;}
        static size_t normal_offset(void) noexcept          {return VN_OFFSET;}
 
        Model(void) = default;
        Model(const std::string& path, const std::string& filename, int flags = 0x0);

        Model(const Model&);
        Model& operator= (const Model&);

        Model(Model&&);
        Model& operator= (Model&&);

        virtual ~Model(void);

        model_error_t load(const std::string& path, const std::string& filename, int flags = 0x0);
        void clear(void) noexcept;

        const std::vector<float>& vertex_data(void) const noexcept                      {return this->v_data;}
        const std::vector<float>& texture_coord_data(void) const noexcept               {return this->vt_data;}
        const std::vector<float>& normal_data(void) const noexcept                      {return this->vn_data;}
        const std::map<std::string, MeshMatierial>& get_materials(void) const noexcept  {return this->materials;}
    };
};

#endif // MODEL_H_INCLUDED
