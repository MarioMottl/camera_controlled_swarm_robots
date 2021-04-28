/******************************************************************************************************************************************
* Title:        Model Loader
* Author:       R-Michi (GitHub name)
* Date:         16.10.2020
* Description:
*   Library for easy model loading for OpenGL.
*   Can only read .obj and .mtl files.
*   The mesh data internally uses 32bit floatingpoint (float).
*
*   The library will try its best to prevent corrupt data by returning errors or throwing exceptions.
*   However, there is a chance that an exception with the text: "Unknown error, is the file corrupt?",
*   will be thrown. If this occurs or the data is corrupt, please check your file and code twice 
*   before reporting this as a bug!
*   
*   If you found a bug and you are !100%! sure that this is in the library, please report it to the 
*   following link: https://github.com/R-Michi/ModelLoader/issues.
*
* @version release 2.0.0
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
******************************************************************************************************************************************/ 

#ifndef __gl_model__
#define __gl_model__

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <exception>

namespace gl
{
    enum model_error_t
    {
        MODEL_ERROR_NONE,
        OBJECT_INVALID_PATH,
        MATERIAL_INVALID_PATH,
        MESH_PRIMITIVE_ERROR,
        MODEL_DATASET_ERROR
    };

    enum model_flag_t : int16_t
    {
        __model_none            = 0x0000,
        __ignore_vt             = 0x0001,
        __ignore_vn             = 0x0002,
        __texture_flip_uv       = 0x0004,
        __gen_normals           = 0x0008,
        __gen_tbn               = 0x0010,
        __normal_x              = 0x0020,
        __normal_y              = 0x0040,
        __normal_z              = 0x0080,
        __data_combined         = 0x0100,
        __cmp_primitive_model   = 0x0200
    };

    enum model_primitive_t : int
    {
        MODEL_PRIMITIVE_NONE    = 0,
        MODEL_POINT             = 1,
        MODEL_LINE              = 2,
        MODEL_TRIANGLE          = 3,
        MODEL_QUAD              = 4,
        MODEL_POLYGON           = 5
    };

    class invalid_material_exception : public std::exception
    {
    private:
        std::string msg;
    
    public:
        invalid_material_exception(void) noexcept = default;
        invalid_material_exception(const std::string& msg)  {this->msg = msg;}

        virtual const char* what(void) const throw()        {return this->msg.c_str();}
    };

    class interpreter_error : public std::exception
    {
    private:
        std::string msg;

    public:
        interpreter_error(void) noexcept = default;
        interpreter_error(const std::string& msg)       {this->msg = msg;}
        
        virtual const char* what(void) const throw()    {return this->msg.c_str();}
    };

    /**
     * class Mesh:
     *  Contains data for rendering: vertices, texture coordinates, normal vectors, tangent- and betangent vectors.
     *  A mesh is only a single region in a big buffer and mostly not the whole buffer, except if there is only one mesh.
     *  It is defined by a begin vertex and a count of vertices. Additionally, there is a read only pointer which
     *  points to the first data of a mesh.
     *  IMPORTANT: There is no check if data is read beyond the buffer range of the mesh. Reading data out of that
     *  range will either result in reading data from another mesh or in a crash!
     */
    class Mesh
    {
    private:
        // begin pointers (read only)
        const float* vertices;
        const float* texcoords;
        const float* normals;
        const float* tangents;
        const float* betangents;
        const float* data;

        // vertex count and begin vertex
        size_t vertex_count;
        size_t begin_vertex;

        // name of material attached to the mesh
        std::string material_name;

        // name of the mesh
        std::string name;

        // type of primitive the mesh contains
        model_primitive_t primitive;

    public:
        Mesh(void) noexcept;

        Mesh(const Mesh&);
        Mesh& operator= (const Mesh&);
        
        Mesh(Mesh&&);
        Mesh& operator= (Mesh&&);

        virtual ~Mesh(void);

        void set_vertex_ptr(const float* vp)        noexcept    {this->vertices = vp;}          // Sets the pointer of the vertices for this mesh.
        void set_texcoord_ptr(const float* vtp)     noexcept    {this->texcoords = vtp;}        // Sets the pointer of the texture coordinates for this mesh.
        void set_normal_ptr(const float* vnp)       noexcept    {this->normals = vnp;}          // Sets the pointer of the normal vectors for this mesh.
        void set_tangent_ptr(const float* tp)       noexcept    {this->tangents = tp;}          // Sets the pointer of the tangent vectors for this mesh.
        void set_betangent_ptr(const float* btp)    noexcept    {this->betangents = btp;}       // Sets the pointer of the betangent vectors for this mesh.
        void set_data_ptr(const float* dp)          noexcept    {this->data = dp;}              // Sets the combined data pointer for this mesh.
        void set_count(size_t c)                    noexcept    {this->vertex_count = c;}       // Sets the vertex count.
        void set_begin(size_t b)                    noexcept    {this->begin_vertex = b;}       // Sets the number of the begin vertex.
        void set_mtlname(const std::string& str)                {this->material_name = str;}    // Sets the material name used for this mesh.
        void set_name(const std::string& str)                   {this->name = str;}             // Sets the name of the mesh
        void set_primitive(model_primitive_t p)     noexcept    {this->primitive = p;}          // Sets the primitive this mesh uses.

        const float* get_vertices(void)         const noexcept  {return this->vertices;}        // Returns a pointer to the vertices for this mesh.
        const float* get_texturecoords(void)    const noexcept  {return this->texcoords;}       // Returns a pointer to the texture coordinates for this mesh.
        const float* get_normals(void)          const noexcept  {return this->normals;}         // Returns a pointer to the normal vectors for this mesh.
        const float* get_tangents(void)         const noexcept  {return this->tangents;}        // Returns a pointer to the tangent vectors for this mesh.
        const float* get_betangents(void)       const noexcept  {return this->betangents;}      // Returns a pointer to the betangent vectors for this mesh.
        const float* get_data(void)             const noexcept  {return this->data;}            // Returns the data pointer for this mesh.
        size_t count(void)                      const noexcept  {return this->vertex_count;}    // Returns the vertex count. (Number of vertices the mesh has.)
        size_t begin(void)                      const noexcept  {return this->begin_vertex;}    // Returns the begin vertex for this mesh.
        const std::string& mtlname(void)        const noexcept  {return this->material_name;}   // Returns the name of the material used for this mesh.
        const std::string& get_name(void)       const noexcept  {return this->name;}            // Returns the name of the mesh.
        model_primitive_t get_primitive(void)   const noexcept  {return this->primitive;}       // Returns the primitive this mesh uses.
    };

    /**
     * class Material:
     *  This class contains light- and material properties and maps.
     *  NOTE: The diffuse map is the regular texture of an object.
     */
    class Material
    {
    private:
        // Material / light properties
        float ambient[4];           // {r, g, b, factor}
        float diffuse[4];           // {r, g, b, factor}
        float specular[4];          // {r, g, b, factor}
        size_t specular_highlight;
        float opacity;

        // Material / light maps
        std::string ambient_map;
        std::string diffuse_map;
        std::string specular_map;
        std::string specular_highlight_map;
        std::string opacity_map;

    public:
        Material(void) noexcept;

        Material(const Material&);
        Material& operator= (const Material&);

        Material(Material&&);
        Material& operator= (Material&&);

        virtual ~Material(void);

        void set_ambient(float r, float g, float b, float f)    noexcept;                                               // Sets the ambient light properies.
        void set_diffuse(float r, float g, float b, float f)    noexcept;                                               // Sets the diffuse light properties.
        void set_specular(float r, float g, float b, float f)   noexcept;                                               // Sets the specular light properties.
        void set_specular_highlight(size_t h)                   noexcept        {this->specular_highlight = h;}         // Sets the specular highlight exponent.
        void set_opacity(float a)                               noexcept        {this->opacity = a;}                    // Sets the alpha value of the material.
        void set_ambient_map(const std::string& am)                             {this->ambient_map = am;}               // Sets the filepath of the ambient map.
        void set_diffuse_map(const std::string& dm)                             {this->diffuse_map = dm;}               // Sets the filepath of the diffuse map.
        void set_specular_map(const std::string& sm)                            {this->specular_map = sm;}              // Sets the filepath of the specular map.
        void set_specular_highlight_map(const std::string& shm)                 {this->specular_highlight_map = shm;}   // Sets the file path of the specular highlight exponent map.
        void set_opacity_map(const std::string& om)                             {this->opacity_map = om;}               // Sets the filepath of the opacity (alpha) map.
        
        void get_ambient_color(float& r, float& g, float& b)    const noexcept;                                         // Returns the r, g, b value of the ambient light color.
        void get_diffuse_color(float& r, float& g, float& b)    const noexcept;                                         // Returns the r, g, b value of the diffuse light color.
        void get_specular_color(float& r, float& g, float& b)   const noexcept;                                         // Returns the r, g, b value of the specular light color.
        float get_ambient_factor(void)                          const noexcept  {return this->ambient[3];}              // Returns the ambient light factor.
        float get_diffuse_factor(void)                          const noexcept  {return this->diffuse[3];}              // Returns the diffuse light factor.
        float get_specular_factor(void)                         const noexcept  {return this->specular[3];}             // Returns the specular light factor.
        size_t get_specular_highlight(void)                     const noexcept  {return this->specular_highlight;}      // Returns the specular highlight exponent.
        float get_opacity(void)                                 const noexcept  {return this->opacity;}                 // Returns the alpha value of the matierial.
        const std::string& get_ambient_map(void)                const noexcept  {return this->ambient_map;}             // Returns the filepath of the ambient map.
        const std::string& get_diffuse_map(void)                const noexcept  {return this->diffuse_map;}             // Reurns the filepath of the diffuse map.
        const std::string& get_specular_map(void)               const noexcept  {return this->specular_map;}            // Returns the filepath of the specular map.
        const std::string& get_specular_highlight_map(void)     const noexcept  {return this->specular_highlight_map;}  // Returns the file path of the specular highlight exponent map.
        const std::string& get_opacity_map(void)                const noexcept  {return this->opacity_map;}             // Returns the filepath of the opacity (alpha) map.
    };

    /**
     * class Model:
     *  The model class contains all meshes and materials read from the model file.
     *  Additionally, it provides values to define / format the mesh data in a way that OpenGL understands.
     *  This class is only able to read object- (.obj) and material files.
     */
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
        static constexpr char NEW_MESH_COMMAND[]                = "o";
        static constexpr char VERTEX_VOMMAND[]                  = "v";
        static constexpr char TEXTURE_COORD_COMMAND[]           = "vt";
        static constexpr char NORMAL_COMMAND[]                  = "vn";
        static constexpr char USE_MATERIAL_COMMAND[]            = "usemtl";
        static constexpr char FACE_COMMAND[]                    = "f";

        // component counts
        static constexpr size_t V_COUNT     = 3;    // component count of vertices (X/Y/Z)
        static constexpr size_t VT_COUNT    = 2;    // component count of texture coordinates (S/T)
        static constexpr size_t VN_COUNT    = 3;    // component count of normal vectors (X/Y/Z)
        static constexpr size_t T_COUNT     = 3;    // component count of tangent vectors (X/Y/Z)
        static constexpr size_t BT_COUNT    = 3;    // component count of betangent vectors (X/Y/Z)

        // split functions for file phraser
        static std::vector<std::string> __split(std::string& str, const char* delimeter);
        static std::vector<std::string> split_line(std::string& line)     {return Model::__split(line, " ");}

        // convert functions
        static float to_float(const std::string& str) noexcept;     // converts string to float
        static std::string to_string(uint32_t i32) noexcept;        // converts uint32_t to string

        // compare functions
        static bool is_comment(const std::string& str) noexcept;    // checks if current line is comment
        static bool is_valid(const std::string& str) noexcept;      // checks if a data set of a face is valid

        // get functions
        static model_primitive_t get_primitive(uint32_t n) noexcept;    // Returns the primitive based on the vertex count of the faces
        static int get_dataset_type(const std::string& data) noexcept;  // Returns a number that corresponds to the dataset type of @param data

        std::map<std::string, Material> _materials;                 // storage of all materials
        std::vector<Mesh> _meshes;                                  // storage of all meshes

        // temporary buffers
        std::vector<float> tmp_v_buff;                              // temporary vertex buffer
        std::vector<float> tmp_vt_buff;                             // temporary texture coordinate buffer
        std::vector<float> tmp_vn_buff;                             // temporary normal vector buffer

        // data-buffers
        std::vector<float> v_buff;                                  // vertex buffer
        std::vector<float> vt_buff;                                 // texture coordinate buffer
        std::vector<float> vn_buff;                                 // normal vector buffer
        std::vector<float> t_buff;                                  // tangent vector buffer
        std::vector<float> bt_buff;                                 // betangent vector buffer
        std::vector<float> data_buff;                               // combined data buffer

        std::string path;                                           // path to the file
        std::string filename;                                       // name of the file

        uint32_t line_counter;                                      // number of the current line in the object or material file
        uint32_t vertex_counter_model;                              // counts the vertices for the model
        uint32_t vertex_counter_mesh;                               // counts the vertives for a mesh
        model_primitive_t mesh_primitive;                           // primitive of the mesh, or of the model
        int dataset_type;                                           // type of the data set for vertices, texture coordinates and normal vectors

        bool has_vertices;                                          // indicator if current mesh contains vertices
        bool has_texcoords;                                         // indicator if current mesh contains texture coordinates
        bool has_normals;                                           // indicator if current mesh contains normal vectors

        void store_v(size_t begin, int flags);                      // copies a vertex from the temprary buffer to the vertex buffer
        void store_vt(size_t begin, int flags);                     // copies a texture coordinate from the temporary buffer to the texture coordinate buffer
        void store_vn(size_t begin, int flags);                     // copies a normal vector from the temporary buffer to the normal vector buffer
        void calc_vn(size_t begin, int flags, size_t count);        // calculates normal vectors from vertices, stores them into the buffer and checks if it is allowed
        void calc_t(size_t begin, int flags);                       // calculates tangent vectors, stores them into the buffer and cheks if it is allowed
        void calc_bt(size_t begin, int flags);                      // calculates betangent vectors, stores them into the buffer and checks if it is allowed
        size_t calc_data_stride(int flags) noexcept;                // calculates the stride if DATA_COMBINED bit is set
        void set_mesh_pointers(size_t idx, int flags);              // Sets all pointers for the mesh based on the flags.

        model_error_t interpret_material_line(const std::vector<std::string>& args, int flags);                 // Interpretes a read line from the material file.
        model_error_t interpret_object_line(const std::vector<std::string>& args, std::string& mtl, int flags); // Interpretes a read line from the object file and returns material filename.
        model_error_t interpret_face(const std::vector<std::string>& args, int flags);                          // Interpretes a face.  

        model_error_t load_object(std::string& mtl, int flags);             // loads data of object file and returns filename of material file
        model_error_t load_material(const std::string& mtl, int flags);     // loads material file and takes as input the material filename   
    
    public:
        // Flags for loading a model.
        // Reference: model_flag.txt
        static constexpr model_flag_t IGNORE_VT             = model_flag_t::__ignore_vt;
        static constexpr model_flag_t IGNORE_VN             = model_flag_t::__ignore_vn;
        static constexpr model_flag_t TEXTURE_FLIP_UV       = model_flag_t::__texture_flip_uv;
        static constexpr model_flag_t GEN_NORMALS           = model_flag_t::__gen_normals;
        static constexpr model_flag_t GEN_TBN               = model_flag_t::__gen_tbn;
        static constexpr model_flag_t NORMAL_X              = model_flag_t::__normal_x;
        static constexpr model_flag_t NORMAL_Y              = model_flag_t::__normal_y;
        static constexpr model_flag_t NORMAL_Z              = model_flag_t::__normal_z;
        static constexpr model_flag_t DATA_COMBINED         = model_flag_t::__data_combined;
        static constexpr model_flag_t CMP_PRIMITIVE_MODEL   = model_flag_t::__cmp_primitive_model;

        // get-component count methods
        static constexpr size_t vertex_component(void)      noexcept {return V_COUNT;}                  // Returns the vertex component count (X/Y/Z).
        static constexpr size_t texcoord_component(void)    noexcept {return VT_COUNT;}                 // Returns the texture coord component count (S/T).
        static constexpr size_t normal_component(void)      noexcept {return VN_COUNT;}                 // Returns the normal vector component count (X/Y/Z).
        static constexpr size_t tangent_component(void)     noexcept {return T_COUNT;}                  // Returns the tangent vector component count (X/Y/Z).
        static constexpr size_t betangent_component(void)   noexcept {return BT_COUNT;}                 // Returns the betangent vector component count (X/Y/Z).

        // get-stride methods. Methods may be useless if combined data is enabled.
        static constexpr size_t vertex_stride(void)         noexcept {return V_COUNT * sizeof(float);}  // Returns the stride of vertices.
        static constexpr size_t texcoord_stride(void)       noexcept {return VT_COUNT * sizeof(float);} // Returns the stride of texture coordinates.
        static constexpr size_t normal_stride(void)         noexcept {return VN_COUNT * sizeof(float);} // Returns the stride of normal vectors.
        static constexpr size_t tangent_stride(void)        noexcept {return T_COUNT * sizeof(float);}  // Returns the stride of tangent vectors.
        static constexpr size_t betangent_stride(void)      noexcept {return BT_COUNT * sizeof(float);} // Returns the stride of betangent vectors.

        static const char* strerror(model_error_t error)    noexcept;                                   // Translates error codes to text.

        Model(void) noexcept : Model("") {}                     // default constructor
        Model(const std::string& path);                         // constructor where path can be set

        Model(const Model&);                                    // copy constructor
        Model& operator= (const Model&);                        // copy operator

        Model(Model&&);                                         // move constructor
        Model& operator= (Model&&);                             // copy operator

        virtual ~Model(void);                                   // destructor

        /**
         *  This method will load the model file (.obj and .mtl).
         *  Additional flags to load the model can be set. To see all aviable flags and what they do
         *  please see the model_flags.txt file in the library folder.
         *  If an error occures while loading, the loading operation will stop immediately.
         *  IMPORTANT: Do not use data if the loading operation fails because the data will be incomplete
         *  and can cause crashes when used.
         *  Following errors can be returned:
         *      MODEL_ERROR_NONE        -> No error occured.
         *      OBJECT_INVALID_PATH     -> Cannot find the path to the GIVEN object (.obj) file.
         *      MATERIAL_INVALID_PATH   -> Cannot find the path to the material (.mtl) file.
         *      MESH_PRIMITIVE_ERROR    -> This error can have two meanings. 
         *                                 First, it indicates that one mesh contains multiple different primitives.
         *                                 Second, it indicates that the whole model file contains multiple different primitives, if CMP_PRIMITIVE_MODEL-bit is set.
         *  Following exception can be thrown.
         *      @exception interpretor_error -> The file interpretor cannot read the current data and will throw an exception.
         *                                      For further information see exception message.
         * 
         *  @param flags    -> Additional loading flags.
         *  @return         -> Loading error.
         */
        model_error_t load(int flags);

        /**
         *  Method to set the path of the model file.
         *  NOTE: Only the path of the object (.obj) file is requiered, the material (.mtl) file should be linked
         *  within the object file.
         *  @param path -> Path to the model (.obj) file.
         *  @exception invalid_material_exception -> Given material name is not aviable.
         */
        void set_path(const std::string& path);

        // Returns the path of the model file.
        std::string get_path(void) const;

        // Returns a vector of meshes that were read from the model file.
        const std::vector<Mesh>& meshes(void) const noexcept  {return this->_meshes;}

        // Returns a material by a given name.
        const Material& matierial(const std::string& name) const;

        size_t data_stride(void)        const noexcept;     // Returns the stride if DATA_COMBINED-bit is set.
        size_t vertex_offset(void)      const noexcept;     // Returns the offset of the vertex data if DATA_COMBINED-bit is set.
        size_t texcoord_offset(void)    const noexcept;     // Returns the offset of the texture coordinates data if DATA_COMBINED-bit is set.
        size_t normal_offset(void)      const noexcept;     // Returns the offset of the normal vectors data if DATA_COMBINED-bit is set.
        size_t tangent_offset(void)     const noexcept;     // Returns the offset of the tangent vectors data if DATA_COMBINED-bit is set.
        size_t betangent_offset(void)   const noexcept;     // Returns the offset of the betangent vectors data if DATA_COMBINED-bit is set.
        // These offsets will always be 0 if DATA_COMBINED-bit is NOT set. 

        void clear(void) noexcept;                          // clears all internal buffers
    };
};

#endif // __gl_model__
