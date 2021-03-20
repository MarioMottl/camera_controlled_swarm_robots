#include "includes/Main.h"

void init_OpenCL(const std::string& device_name_regex, cl_command_queue& cmd_queue, cl_context& context)
{
    cmd_queue = NULL;

    // get platforms
    unsigned int num_platforms;
    clGetPlatformIDs(0, NULL, &num_platforms);
    cl_platform_id platforms[num_platforms];
    int err = clGetPlatformIDs(num_platforms, platforms, NULL);

    if(err != 0)
    {
        std::cout << get_msg("ERROR / INIT OpenCL") << "Failed to load platforms. OpenCL error: " << err << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / INIT OpenCL") << "Platforms loaded (" << num_platforms << ")." << std::endl;

    cl_device_id device = NULL;
    cl_platform_id platform = NULL;
    bool found_matching_GPU = false;

    //get right device & platform
    for(unsigned int i=0; i<num_platforms && !found_matching_GPU; i++)
    {

        // get devices
        unsigned int num_devices;
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
        cl_device_id devices[num_devices];
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);

        if(err != 0)
        {
            std::cout << get_msg("ERROR / INIT OpenCL") << "Failed to load devices. OpenCL error: " << err << std::endl;
            exit(-1);
        }
        std::cout << get_msg("INFO / INIT OpenCL") << "Devices loaded (" << num_devices << ") for platform " << i << "." << std::endl;

        if(err == 0)
        {
            // search for right device
            for(unsigned int j=0; j<num_devices && !found_matching_GPU; j++)
            {
                // get device info
                char info[64];
                clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(info), info, NULL);

                std::string str = info;
                int found = str.find(device_name_regex, 0);
                if(found != -1)
                {
                    found_matching_GPU = true;
                    platform = platforms[i];
                    device = devices[j];
                }
            }
        }
    }

    if(device == NULL)
    {
        std::cout << get_msg("ERROR / INIT OpenCL") << "No device found with regex: \"" + device_name_regex + "\"." << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / INIT OpenCL") <<  "Device found with regex: \"" + device_name_regex + "\"." << std::endl;

    char info[64];
    clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(info), info, NULL);
    std::cout << get_msg("INFO / INIT OpenCL") <<  "Used platform name: \"" << info << "\"."  << std::endl;

    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(info), info, NULL);
    std::cout << get_msg("INFO / INIT OpenCL") <<  "Used device name: \"" << info << "\"."  << std::endl;

    cl_context_properties properties[] = {
	    CL_GL_CONTEXT_KHR,      (cl_context_properties)wglGetCurrentContext(),
	    CL_WGL_HDC_KHR,         (cl_context_properties)wglGetCurrentDC(),
        CL_CONTEXT_PLATFORM,    (cl_context_properties)platform,
	    0
    };

    context = clCreateContext(properties, 1, &device, NULL, NULL, &err);
    if(err != 0)
    {
        std::cout << get_msg("ERROR / INIT OpenCL") << "Failed to create OpenCL context. OpenCL error: " << err << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / INIT OpenCL") << "OpenCL context created." << std::endl;

    cmd_queue = clCreateCommandQueue(context, device, 0, &err);
    if(err != 0)
    {
        std::cout << get_msg("ERROR / INIT OpenCL") << "Failed to create OpenCL command queue." << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / INIT OpenCL") << "OpenCL command queue created." << std::endl;
}

void init_OpenGL(GLFWvidmode& vid_mode, WindowHandler& main_window_handler, int interval)
{
    glfwInit();
    glfwGetDesktopMode(&vid_mode);
    main_window_handler.init(vid_mode.Width / 2, vid_mode.Height / 2);

    std::cout << get_msg("INFO / OpenGL") << "GLEW initialized." << std::endl;
    if(!glfwOpenWindow(vid_mode.Width / 2, vid_mode.Height / 2, 24, 24, 24, 24, 24, 8, GLFW_WINDOW))
    {
        std::cout << get_msg("ERROR / OpenGL WINDOW") << "Failed to create OpenGL window." << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwSetWindowPos(vid_mode.Width / 4, vid_mode.Height / 4);
    glfwSetWindowTitle("Scharm GUI und Visualisierung - 0.0.0");
    std::cout << get_msg("INFO / OpenGL WINDOW") << "OpenGL window created." << std::endl;
    glewInit();
    std::cout << get_msg("INFO / OpenGL") << "GLFW initialized." << std::endl;
    glfwSwapInterval(interval);    // limit to 60 fps
}

void init_event_handler(EventHandler& event_handler, Schwarm::Client::SharedSimulationMemory* mem)
{
    TextInpListener* button_listener = new TextInpListener();
    button_listener->set_sharedsimumem(mem);
    event_handler.add_listener(button_listener);
    std::cout << get_msg("INFO / EVENT-HANDLER") << "Initialized Event-Handler." << std::endl;
}

void load_button_font(gl::Font& font)
{
    constexpr char path_font[32] = "Font/arial.png";
    constexpr char path_offset[32] = "Font/arial_offset.txt";

    std::cout << get_msg("INFO / LOADING FONT") << "Loading font offsets \"" << path_offset << "\"." << std::endl;
    std::fstream font_offset(path_offset, std::ios::in);
    std::vector<gl::CharacterOffset> offsets;

    if(!font_offset)
        std::cout << get_msg("INFO / LOADING FONT") << "Unable open file \"" << path_offset << "\"." << std::endl;

    // read offsets from file
    gl::CharacterOffset offset;
    while(!font_offset.eof())
    {
        font_offset >> offset.positive_x >> offset.positive_y >> offset.negative_x >> offset.negative_y;
        offsets.push_back(offset);
    }
    font_offset.close();

    std::cout << get_msg("INFO / LOADING FONT") << "Loading font \"" << path_font << "\"." << std::endl;
    gl::FontError err = font.load(path_font, true, offsets);
    if(err == gl::FontError::FONT_ERROR_INVALID_PATH)
    {
        std::cout << get_msg("ERROR / LOADING FONT") << "Unable to find path \"" << path_font << "\"." << std::endl;
        exit(-1);
    }
    else if(err == gl::FontError::FONT_ERROR_INVALID_IMAGE_SIZE)
    {
        std::cout << get_msg("ERROR / LOADING FONT") << "Button font has invalid image size." << std::endl;
        exit(-1);
    }
    else if(err == gl::FontError::FONT_ERROR_INVALID_NUM_CHARS)
    {
        std::cout << get_msg("ERROR / LOADING FONT") << "Button font has invalid number of characters." << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / LOADING FONT") << "Button font loaded." << std::endl;
}

void load_texture(unsigned int& texture, const std::string path, bool use_gamma)
{
    std::cout << get_msg("INFO / TEXTURE") << "Loading texture \"" << path << "\"..." << std::endl;

    int x_pixels, y_pixels;
    uint8_t* data = stbi_load(path.c_str(), &x_pixels, &y_pixels, NULL, 4);  // always load 4 components because the internal format requieres RGBA

    if(data == NULL)
    {
        std::cout << get_msg("ERROR / TEXTURE") << "Unable to load texture \"" << path << "\"." << std::endl;
        exit(-1);
    }
    else
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, (use_gamma) ? GL_SRGB_ALPHA : GL_RGBA, x_pixels, y_pixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
        std::cout << get_msg("INFO / TEXTURE") << "Texture \"" << path << "\" loaded." << std::endl;
    }
}

void load_shader(gl::Shader& shader, const std::string& vertex_fname, const std::string& fragment_fname)
{
    static const std::string path("Shaders/");
    std::cout << get_msg("INFO / LOADING SHADER") << "Loading shader \"" << path + vertex_fname << "\" and \"" << path + fragment_fname << "\"..." << std::endl;
    gl::ShaderLoadError err = shader.load(path + vertex_fname, path + fragment_fname);
    if(err & gl::ShaderLoadErrorType::INVALID_FILE_PATH)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Unable to find path \"" << path + vertex_fname << "\" or \"" << path + fragment_fname << "\"." << std::endl;
        exit(-1);
    }
    if(err & gl::ShaderLoadErrorType::SHADER_ALREADY_LOADED)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Shader has already been loaded." << std::endl;
        exit(-1);
    }
    if(err & gl::ShaderLoadErrorType::VERTEX_SHADER_ERROR)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Compile error occured in vertex shader." << std::endl;
        std::cout << get_msg("INFO / VERTEX SHADER") << shader.get_last_vertex_msg() << std::endl;
        exit(-1);
    }
    if(err & gl::ShaderLoadErrorType::FRAGMENT_SHADER_ERROR)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Compile error occured in fragment shader." << std::endl;
        std::cout << get_msg("INFO / FRAGMENT SHADER") << shader.get_last_fragment_msg() << std::endl;
        exit(-1);
    }
    if(err & gl::ShaderLoadErrorType::SHADER_LINK_ERROR)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Link error occured." << std::endl;
        std::cout << get_msg("INFO / LINK SHADER") << shader.get_last_link_msg() << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / LOADING SHADER") << "Shader \"" << path + vertex_fname << "\" and \"" << path + fragment_fname << "\" loaded." << std::endl;
}

int min_time(const std::vector<int>& vec)
{
    int min = vec[0];
    for(int i : vec)
    {
        if(min > i)
            min = i;
    }
    return min;
}

int max_time(const std::vector<int>& vec)
{
    int max = vec[0];
    for(int i : vec)
    {
        if(max < i)
            max = i;
    }
    return max;
}

double avg_time(const std::vector<int>& vec)
{
    double sum = 0.0f;
    for(int i : vec)
        sum += i;
    return sum / vec.size();
}

void update_camera(glm::dmat4& view, glm::dmat4& projection, const WindowHandler& main_window_handler, const WindowHandler& vis_window_handler)
{
    /*  The width and height of the main window is used eventhough the 3D render scene is a seperate framebuffer.
    *   The reason for that is the mouse is always locked to the main window and never to a seperate framebuffer. */
   if(Main::get_view_state() == Main::ViewState::OPEN_WORLD)
   {
        Main::mouse_action(main_window_handler.get_width(), main_window_handler.get_height(), Main::camera_location().rotx, Main::camera_location().roty, Main::get_sensetivity());
        Main::move_action(Main::camera_location().x, Main::camera_location().y, Main::camera_location().z, Main::camera_location().rotx, Main::get_move_speed());
   }

    view = glm::lookAt(glm::dvec3(Main::camera_location().x, Main::camera_location().y, Main::camera_location().z),
                       glm::dvec3(Main::camera_location().x + sin(Main::camera_location().rotx) * cos(Main::camera_location().roty),
                                  Main::camera_location().y + sin(Main::camera_location().roty),
                                  Main::camera_location().z + cos(Main::camera_location().rotx) * cos(Main::camera_location().roty)),
                       glm::dvec3(0.0, 1.0, 0.0));

    projection = glm::perspective(glm::radians(Main::get_fov()), vis_window_handler.get_aspect(), 0.001f, 500.0f);
}

/* -----------------------------------------------------------------------------
 * GENERATE-BUFFER-FUNCTIONS
 * -----------------------------------------------------------------------------*/

unsigned int gen_scene_fb(const GLFWvidmode& vid_mode, unsigned int& tex)
{
    unsigned int fbo_scene;
    glGenFramebuffers(1, &fbo_scene);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_scene);

    // create color attachment
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vid_mode.Width, vid_mode.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "incomplete framebuffer (Render scene)" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fbo_scene;
}

unsigned int gen_vis_fb(const GLFWvidmode& vid_mode, unsigned int& tex, unsigned int& rb)
{
    unsigned int fbo_vis;
    glGenFramebuffers(1, &fbo_vis);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_vis);

    // create color attachment (texture)
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vid_mode.Width, vid_mode.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    
    // create depth and stencil attachment (renderbuffer)
    glGenRenderbuffers(1, &rb);
    glBindRenderbuffer(GL_RENDERBUFFER, rb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, vid_mode.Width, vid_mode.Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "incomplete framebuffer (visualization)" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fbo_vis;
}

unsigned int gen_buffer_fb_scene(float** map, size_t data_size, unsigned int& vbo)
{
    unsigned int vao_fb;
    glGenVertexArrays(1, &vao_fb);
    glBindVertexArray(vao_fb);

    // create buffer for framebuffer quads
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferStorage(GL_ARRAY_BUFFER, data_size, NULL, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT);
    *map = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, data_size, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, data_size);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(0 * sizeof(float)));
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return vao_fb;
}

unsigned int gen_table_buffer(gl::Model& model, unsigned int* vbos)
{
    static const size_t MODEL_STRIDE = sizeof(float) * (gl::Model::vertex_size() + gl::Model::texture_coord_size() + gl::Model::normal_size());

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(2, vbos);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.vertex_data().size(), model.vertex_data().data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, gl::Model::vertex_size(),          GL_FLOAT, false, MODEL_STRIDE, (void*)(gl::Model::vertex_offset()));
    glVertexAttribPointer(1, gl::Model::texture_coord_size(),   GL_FLOAT, false, MODEL_STRIDE, (void*)(gl::Model::texture_coord_offset()));
    glVertexAttribPointer(2, gl::Model::normal_size(),          GL_FLOAT, false, MODEL_STRIDE, (void*)(gl::Model::normal_offset()));

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), glm::value_ptr(glm::mat4(1.0f)), GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);   // 3 + 0
    glEnableVertexAttribArray(4);   // 3 + 1
    glEnableVertexAttribArray(5);   // 3 + 2
    glEnableVertexAttribArray(6);   // 3 + 3
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(0 * sizeof(glm::vec4)));
    glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
    glVertexAttribPointer(5, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glVertexAttribPointer(6, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    model.clear();
    return vao;
}

unsigned int gen_depth_map_fb(unsigned int& shadow_map, unsigned int resolution)
{
    glGenTextures(1, &shadow_map);
    glBindTexture(GL_TEXTURE_2D, shadow_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "incomplete framebuffer (Render scene)" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fbo;
}

/* -----------------------------------------------------------------------------
* UPDATE-FUNCTIONS
* -----------------------------------------------------------------------------*/

void update_scenes(unsigned int vbo, float* fb_data, const GLFWvidmode& vid_mode, const WindowHandler& main_wh, const WindowHandler& vis_wh)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Main::vis_scene_data(fb_data, vis_wh, vid_mode);
    Main::render_scene_data(fb_data, main_wh, vid_mode);
    glFlushMappedNamedBufferRange(GL_ARRAY_BUFFER, 0, 2 * Main::fb_data_size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void update_GUI_matrices(glm::mat4& projection, gl::Shader& text_shader, const WindowHandler& main_wh)
{
    projection = glm::ortho(-main_wh.get_aspect(), main_wh.get_aspect(), -1.0f, 1.0f, -1.0f, 1.0f);
    text_shader.use();
    text_shader.uniform_matrix_4x4f("projection2D", 1, false, glm::value_ptr(projection));
    text_shader.uniform_1f("th0", 0.40f);
    text_shader.uniform_1f("th1", 0.55f);
}

/* -----------------------------------------------------------------------------
* DRAW- / RENDER-FUNCTIONS
* -----------------------------------------------------------------------------*/

void set_light_values(gl::Shader& shader)
{
    shader.use();
    shader.uniform_3f("camPos", Main::camera_location().x, Main::camera_location().y, Main::camera_location().z);

    shader.uniform_3f("worldLight.direction", Main::worldlight_position().x, Main::worldlight_position().y, Main::worldlight_position().z);
    shader.uniform_3f("worldLight.color", 1.0f, 1.0f, 1.0f);
    shader.uniform_1f("worldLight.ambientStrength", Main::apply_gamma(0.25f, Main::get_gamma()));
    shader.uniform_1f("worldLight.diffuseStrength", Main::apply_gamma(0.75f, Main::get_gamma()));
    shader.uniform_1f("worldLight.specularStrength", 1.0f);
    shader.uniform_1i("worldLight.shadowMap", 0);   // shadow map for worldLight: active texture 0

    #if 0
    shader.uniform_3f("lampSpotlight.direction", 0.0f, -1.0, 0.0); // facing downwards
    shader.uniform_3f("lampSpotlight.position", 0.0f, 1.565f, 0.0f); 
    shader.uniform_3f("lampSpotlight.color", 1.0f, Main::apply_gamma(0.871f, Main::get_gamma()), Main::apply_gamma(0.678f, Main::get_gamma()));
    shader.uniform_1f("lampSpotlight.innerAngle", 0.766f);  // 40°
    shader.uniform_1f("lampSpotlight.outherAngle", 0.642f); // 50°
    shader.uniform_1f("lampSpotlight.ambientStrength", 0.0f);
    shader.uniform_1f("lampSpotlight.diffuseStrength", 0.0f);
    shader.uniform_1f("lampSpotlight.specularStrength", 0.0f);
    shader.uniform_1f("lampSpotlight.constant", 1.0f);
    shader.uniform_1f("lampSpotlight.linear", 0.09f);
    shader.uniform_1f("lampSpotlight.quadratic", 0.032f);
    #endif
}

void draw_table(gl::Shader& shader, const gl::MeshMatierial& table_mtl, unsigned int vao_table, unsigned int diffuse_map, unsigned int specular_map, unsigned int* shadow_maps)
{
    // material values for table
    shader.use();
    shader.uniform_1i("material.isSource", 0);
    shader.uniform_1i("material.diffuse", 1);   // ambient and diffuse map (object color): active texture 1
    shader.uniform_1i("material.specular", 2);  // specular map: active texture 2

    glBindVertexArray(vao_table);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadow_maps[0]);   // bind worldlight shadow map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffuse_map);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specular_map);

    glDrawArrays(GL_QUADS, table_mtl.vertex_offset, table_mtl.vertex_count);

    glBindTexture(GL_TEXTURE_2D, 0);    // unbind texture 2 (spacular map)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);    // unbind texture 1 (diffuse and ambient map)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);    // unbind texture 0 (worldlight shadow map)
    glBindVertexArray(0);
}

void draw_vehicles(gl::Shader& shader, const gl::MeshMatierial& mtl, unsigned int vao_vehicle, unsigned int n, unsigned int diffuse_map, unsigned int specular_map, unsigned int* shadow_maps)
{
    shader.use();
    // use the same material properties as for table

    glBindVertexArray(vao_vehicle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadow_maps[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffuse_map);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specular_map);

    glDrawArraysInstanced(GL_QUADS, mtl.vertex_offset, mtl.vertex_count, n);
    
    glBindTexture(GL_TEXTURE_2D, 0);    // unbind texture 2 (spacular map)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);    // unbind texture 1 (diffuse and ambient map)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);    // unbind texture 0 (worldlight shadow map)
    glBindVertexArray(0);
}

void draw_GUI(gl::Shader& e_shader, gl::Shader& t_shader, GUI::ElementRenderer& e_renderer, const gl::Font& font)
{
    e_shader.use();
    glBindVertexArray(e_renderer.get_element_vertex_array());
    glDrawArrays(GL_QUADS, 0, e_renderer.get_element_vertex_count());

    t_shader.use();
    glBindVertexArray(e_renderer.get_text_vertex_array());
    glBindTexture(GL_TEXTURE_2D, font.texture_id());
    glMultiDrawArraysIndirect(GL_QUADS, e_renderer.get_text_command(), e_renderer.get_text_count(), e_renderer.get_text_command_stride());
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void draw_fb(gl::Shader& shader, unsigned int vao_fb, unsigned int tex, size_t begin)
{
    shader.use();
    glBindVertexArray(vao_fb);
    glBindTexture(GL_TEXTURE_2D, tex);

    glDrawArrays(GL_QUADS, begin, begin + 4); // vertices 0 - 3 = vis framebuffer

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void draw_scene(gl::Shader& scene_shader, unsigned int vao_fb, unsigned int tex, size_t begin)
{
    scene_shader.use();
    // set post-processing values
    scene_shader.uniform_1f("gamma", Main::get_gamma());
    scene_shader.uniform_1i("autoGammaCorrection", Main::auto_gamma());
        
    glBindVertexArray(vao_fb);
    glBindTexture(GL_TEXTURE_2D, tex);

    glDrawArrays(GL_QUADS, begin, begin + 4);   // vertices 4 - 7 = render scene framebuffer

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

int main()  // its showtime
{
    constexpr unsigned int WORLDLIGHT_SHADOW_RESOLUTION = 2048;
    bool main_running = true;
    std::atomic_bool simu_running = false;

    Schwarm::Client::SharedSimulationMemory sharedsimumem;

    /* -----------------------------------------------------------------------------
     * SHADER FILENAMES
     * -----------------------------------------------------------------------------*/
    const std::string element_vs("element.vert");
    const std::string element_fs("element.frag");
    const std::string text_vs("text.vert");
    const std::string text_fs("text.frag");
    const std::string vis_vs("vis.vert");
    const std::string vis_fs("vis.frag");
    const std::string fb_scene_vs("fb_scene.vert");
    const std::string fb_scene_fs("fb_scene.frag");
    const std::string scene_vs("scene.vert");
    const std::string scene_fs("scene.frag");
    const std::string sds_vs("simple_depth.vert");
    const std::string sds_fs("simple_depth.frag");

    /* -----------------------------------------------------------------------------
     * DECLARE WINDOW HANDLERS
     * -----------------------------------------------------------------------------*/
    WindowHandler main_window_handler, vis_window_handler;
    
    /* -----------------------------------------------------------------------------
     * INIT EVENT-HANDLER
     * -----------------------------------------------------------------------------*/
    EventHandler event_handler(ListenerType::DYNAMIC_LISTENER);
    init_event_handler(event_handler, &sharedsimumem);
    event_handler.start();
    std::cout << get_msg("INFO / EVENT-HANDLER") << "Started Event-Handler." << std::endl;

    /* -----------------------------------------------------------------------------
     * LOAD MODELS
     * -----------------------------------------------------------------------------*/
    gl::Model table;
    gl::model_error_t model_error = table.load("Models/", "table.obj", gl::Model::MESH_COMBINED | gl::Model::INVERT_T);
    gl::Model vehicle;
    model_error = vehicle.load("Models/", "vehicle.obj", gl::Model::MESH_COMBINED | gl::Model::INVERT_T);

    const gl::MeshMatierial table_mtl = table.get_materials().begin()->second;         // there is only one material
    const gl::MeshMatierial vehicle_mtl = vehicle.get_materials().begin()->second;

    if(model_error != gl::model_error_t::MODEL_ERROR_NONE)
    {
        return -1;
    }

    /* -----------------------------------------------------------------------------
     * Start server and client.
     * ----------------------------------------------------------------------------*/

    // setup scoket handler
    SH::SocketHandler sockethandler;
    sockethandler.start();
    std::cout << get_msg("INFO / CLIENT") << "Started socket-handler." << std::endl;

    // setup client
    SH::Client client;
    client.set_callbacks(Schwarm::Client::on_pathsimu_connect, Schwarm::Client::on_pathsimu_disconnect, Schwarm::Client::on_pathsimu_receive);
    client.set_sockethandler(sockethandler);
    *client.ersist_ptr() = &sharedsimumem;

    // set client to shared simulation memory
    sharedsimumem.client = &client;

    // start simulation server
    const std::string imagefolder("E:/Privat/Programmieren/Diplomarbeit/SchwarmGUI/Images");
    Schwarm::Client::start_pathserver(&simu_running, &imagefolder);
    std::cout << get_msg("INFO / SERVER") << "Started simulation server." << std::endl;
    sharedsimumem.start = false;
    sharedsimumem.client->connect("10.212.134.6", 10005);
    std::cout << get_msg("INFO / SERVER") << "Connected to simulation server (Address: " << Schwarm::SIMU_SERVER_ADDR << " Port: " << Schwarm::SIMU_SERVER_PORT << ")" << std::endl;

    // for test send verification code
    client.get_socket().send("Michi\0", 6, 0);

    /* -----------------------------------------------------------------------------
     * INIT OpenGL
     * ----------------------------------------------------------------------------*/
    GLFWvidmode vid_mode;
    init_OpenGL(vid_mode, main_window_handler, 1);

    /* -----------------------------------------------------------------------------
     * INIT OpenCL
     * -----------------------------------------------------------------------------*/
    cl_command_queue cmd_queue;
    cl_context context;
    init_OpenCL("GeForce", cmd_queue, context);

    /* -----------------------------------------------------------------------------
     * INIT MAIN.H AND WINDOW-HANDLERS
     * -----------------------------------------------------------------------------*/
    Main::camera_location() = {0.0, 0.5, 0.0, 0.0, 0.0};
    Main::set_fov(70.0f);

    Main::set_sensetivity(0.002f);
    Main::set_move_speed(0.5f);

    Main::set_gamma(2.2f);
    Main::auto_gamma() = false;
    Main::reload_colors();

    glm::vec3 wl_pos(1.59, 2.981, 1.193);
    Main::transform_lightpos(wl_pos, 3.5f);
    Main::worldlight_position() = wl_pos;

    Main::set_vis_pos(1.0f, 0.85f);
    Main::set_vis_width(2.0f);
    Main::set_vis_height(1.75f);
    vis_window_handler.init(gl::convert::to_pixels_size(Main::get_vis_width(), main_window_handler.get_width()), 
                            gl::convert::to_pixels_size(Main::get_vis_height(), main_window_handler.get_height()));

    Main::set_table_size(1.84f, 0.89f);
    Main::set_table_origin(-0.92, -0.445);

    /* -----------------------------------------------------------------------------
     * LOAD FONTS AND TEXTURES
     * -----------------------------------------------------------------------------*/
    // used font
    gl::Font element_font;
    load_button_font(element_font);

    // load texture for table (ambient / diffuse and specular map)
    unsigned int table_diffuse_map;
    load_texture(table_diffuse_map, "Textures/table_diffuse_ambient_map.png", true);
    unsigned int table_specular_map;
    load_texture(table_specular_map, "Textures/table_specular_map.png", false);
    unsigned int vehicle_diffuse_map;
    load_texture(vehicle_diffuse_map, "Textures/vehicle_diffuse_ambient_map.png", true);
    unsigned int vehicle_specular_map;
    load_texture(vehicle_specular_map, "Textures/vehicle_specular_map.png", false);

    /* -----------------------------------------------------------------------------
     * LOAD SHADERS
     * -----------------------------------------------------------------------------*/
    gl::Shader element_shader(256), text_shader(256), fb_scene_shader(256), vis_shader(256), scene_shader(256), simple_depth_shader(256);
    load_shader(element_shader,         element_vs,     element_fs);
    load_shader(text_shader,            text_vs,        text_fs);
    load_shader(fb_scene_shader,        fb_scene_vs,    fb_scene_fs);
    load_shader(vis_shader,             vis_vs,         vis_fs);
    load_shader(scene_shader,           scene_vs,       scene_fs);
    load_shader(simple_depth_shader,    sds_vs,         sds_fs);

    /* -----------------------------------------------------------------------------
     * CREATE FRAMEBUFFER FOR RENDER SCENE
     * -----------------------------------------------------------------------------*/
    unsigned int tex_scene;
    unsigned int fbo_scene = gen_scene_fb(vid_mode, tex_scene);
    
    /* -----------------------------------------------------------------------------
     * CREATE FRAMEBUFFER FOR VISUALIZATION SCENE
     * -----------------------------------------------------------------------------*/
    unsigned int tex_vis, rbo_vis;
    unsigned int fbo_vis = gen_vis_fb(vid_mode, tex_vis, rbo_vis);

    /* -----------------------------------------------------------------------------
     * CREATE FRAMEBUFFER(S) FOR SHADOW MAPS
     * -----------------------------------------------------------------------------*/
    unsigned int shadow_maps[1];    // 0: worldlight shadow map
    unsigned int fbo_worldlight_shadow = gen_depth_map_fb(shadow_maps[0], WORLDLIGHT_SHADOW_RESOLUTION);
    std::cout << get_msg("INFO / OpenGL") << "Created and loaded framebuffers." << std::endl;

    /* -----------------------------------------------------------------------------
     * CREATE BUFFER FOR FRAMEBUFFER(S)
     * -----------------------------------------------------------------------------*/
    float* map_fb_data;
    unsigned int vbo_fb;
    unsigned int vao_fb = gen_buffer_fb_scene(&map_fb_data, 2 * Main::fb_data_size(), vbo_fb);

    // fill buffer with data
    Main::vis_scene_data(map_fb_data, vis_window_handler, vid_mode);
    Main::render_scene_data(map_fb_data, main_window_handler, vid_mode);
    std::cout << get_msg("INFO / OpenGL") << "Render-quads for framebuffer-scenes loaded." << std::endl;
    
    /* -----------------------------------------------------------------------------
     * CREATE BUFFER FOR TABLE
     * -----------------------------------------------------------------------------*/
    unsigned int vbo_table[2];
    unsigned int vao_table = gen_table_buffer(table, vbo_table);
    std::cout << get_msg("INFO / OpenGL") << "Table created and loaded." << std::endl;

    /* -----------------------------------------------------------------------------
     * CREATE AND INIT GUI
     * -----------------------------------------------------------------------------*/
    // set font for elements
    std::cout << get_msg("INFO / GUI") << "Initializing GUI..." << std::endl;
    GUI::Element::set_font(&element_font);

    /* CREATE BUTTONS */
    std::cout << get_msg("INFO / GUI") << "Creating buttons..." << std::endl;
    GUI::Button stop_button(main_window_handler.get_width_ptr(), main_window_handler.get_height_ptr(), main_window_handler.get_aspect_ptr());
    stop_button.set_pos(-0.975f, 0.875f);
    stop_button.set_size(0.2f, 0.1f);
    stop_button.set_button_color(Main::BUTTON_STD_COLOR);
    stop_button.set_text_value("STOP");
    stop_button.set_font_size(0.075f);
    stop_button.set_text_color(Main::BUTTON_TEXT_COLOR);
    std::cout << get_msg("INFO / GUI") << "Buttons created." << std::endl;
    
    /* CREATE TEXTBOXES */
    std::cout << get_msg("INFO / GUI") << "Creating textboxes..." << std::endl;
    GUI::TextBox cmd_line(main_window_handler.get_width_ptr(), main_window_handler.get_height_ptr(), main_window_handler.get_aspect_ptr());
    cmd_line.set_pos(-0.975f, -1.0f);
    cmd_line.set_size(1.95f, 0.1f);
    cmd_line.set_textinput_color(Main::TEXTBOX_ACTIVE_COLOR);
    cmd_line.set_cursor_color(Main::TEXTBOX_CURSOR_COLOR);
    cmd_line.set_font_size(0.075f);
    cmd_line.set_text_color(Main::TEXTBOX_ACTIVE_TEXT_COLOR);
    std::cout << get_msg("INFO / GUI") << "Textboxes created..." << std::endl;

    /* -----------------------------------------------------------------------------
     * INIT ELEMENT HANDLER
     * -----------------------------------------------------------------------------*/
    GUI::ElementHandler element_handler;
    element_handler.attach_element(stop_button);
    element_handler.attach_element(cmd_line);
    element_handler.start(std::chrono::milliseconds(5));
    std::cout << get_msg("INFO / GUI") << "Started Element-Handler." << std::endl;

    /* -----------------------------------------------------------------------------
     * INIT ELEMENT RENDERER
     * -----------------------------------------------------------------------------*/
    GUI::ElementRenderer element_renderer(context, cmd_queue);
    element_renderer.attach_handler(element_handler);
    element_renderer.init();
    element_renderer.start(std::chrono::milliseconds(5));
    std::cout << get_msg("INFO / GUI") << "Started Element-Renderer." << std::endl;
    std::cout << get_msg("INFO / GUI") << "Successfully initialized GUI." << std::endl;

    std::cout << get_msg("INFO / OpenGL") << "Load scene..." << std::endl;
    
    /* -----------------------------------------------------------------------------
     * CREATE VEHICLES (BUFFERS)
     * -----------------------------------------------------------------------------*/

    Schwarm::Vehicle vehicle1, vehicle2, vehicle3;  // initialize vehicles
    vehicle1.translate(0.25f, 0.015f, 0.0f);
    vehicle1.set_speed(0.2f);
    vehicle2.translate(-0.25f, 0.015f, 0.25f);
    vehicle2.set_speed(0.2f);
    vehicle3.translate(-0.25, 0.015f, -0.25);
    vehicle3.set_speed(0.5f);
    vehicle1.calc();
    vehicle2.calc();
    vehicle3.calc();
    std::cout << get_msg("INFO / OpenGL") << "Vehicles created." << std::endl;

    /*
    *   NOTE: The vehicle-buffer only keeps a pointer to the vehicle.
    *   Deleting the vehicle will cause a crash.
    *   For vehicles allocated dynamically set the "dynamic_vehicles" parameter to true.
    *   The vehicle-buffer will also delete the allocated memory for the vehicles.
    */
    Schwarm::VehicleBuffer vehicle_buffer(context, cmd_queue, vehicle, false, 3);
    vehicle_buffer.add_vehicle(&vehicle1);  // add vehicles to the buffer
    //vehicle_buffer.add_vehicle(&vehicle2);
    //vehicle_buffer.add_vehicle(&vehicle3);
    std::cout << get_msg("INFO / OpenGL") << "Vehicles loaded." << std::endl;

    // Start vehicle-processor.
    Schwarm::VehicleProcessor vehicle_processor(&sharedsimumem);
    vehicle_processor.set_tablesize(Main::get_table_size_x(), Main::get_table_size_y());
    vehicle_processor.set_tableorigin(Main::get_table_origin_x(), Main::get_table_origin_y());
    vehicle_processor.set_buffer(&vehicle_buffer);
    vehicle_processor.set_tickspeed(std::chrono::milliseconds(10));
    vehicle_processor.start();

    /* -----------------------------------------------------------------------------
     * DECLARE & INIT OpenGL RENDER MATRICES
     * -----------------------------------------------------------------------------*/

    /* GUI PROJECTION MATRICES */
    glm::mat4 projection2D;
    std::cout << get_msg("INFO / OpenGL") << "GUI-matrices created." << std::endl;

    /* VISUALIZATION SCENE MATRICES */
    glm::dmat4 vis_projection;
    glm::dmat4 vis_view;
    std::cout << get_msg("INFO / OpenGL") << "Visualization-scene-matrices created." << std::endl;

    /* LIGHT SPACE MATRICES */
    const float near_plane = 1.0f, far_plane = 5.0f;
    glm::mat4 worldlight_projection = glm::ortho(-1.7f, 1.7f, -1.7f, 1.7f, near_plane, far_plane);
    glm::mat4 worldlight_view = glm::lookAt(Main::worldlight_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 light_space_transform = worldlight_projection * worldlight_view;
    std::cout << get_msg("INFO / OpenGL") << "Light-space-matrices for visualization-scene created." << std::endl;
    std::cout << get_msg("INFO / OpenGL") << "Scene loaded successully." << std::endl;
    
    /* -----------------------------------------------------------------------------
     * TIME MEASUREMENT QUERIES
     * -----------------------------------------------------------------------------*/
    unsigned int timer_query;
    glGenQueries(1, &timer_query);
    std::vector<int> time_results;

    while(main_running)
    {
        glBeginQuery(GL_TIME_ELAPSED, timer_query);

        /* UPDATE RENDERERS / HANDLERS */
        element_renderer.update();

        /* UPDATE WINDOW-HANDLERS */
        int main_width, main_height;
        glfwGetWindowSize(&main_width, &main_height);
        main_window_handler.update(main_width, main_height, true);
        vis_window_handler.update(gl::convert::to_pixels_size(Main::get_vis_width(), main_window_handler.get_width()), 
                                  gl::convert::to_pixels_size(Main::get_vis_height(), main_window_handler.get_height()), false);

        /* UPDATE SCEENES */
        update_scenes(vbo_fb, map_fb_data, vid_mode, main_window_handler, vis_window_handler);

        /* UPDATE MATRICES & KEY EVENTS*/
        Main::process_key_events();
        update_GUI_matrices(projection2D, text_shader, main_window_handler);
        update_camera(vis_view, vis_projection, main_window_handler, vis_window_handler);

        /* -----------------------------------------------------------------------------
         * VISUALIZATION SCENE
         * -----------------------------------------------------------------------------*/
        // set flags
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        /* GENERATE (DRAW) SHADOW MAP(S) */
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_worldlight_shadow);
        glViewport(0, 0, WORLDLIGHT_SHADOW_RESOLUTION, WORLDLIGHT_SHADOW_RESOLUTION);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        simple_depth_shader.use();
        simple_depth_shader.uniform_matrix_4x4f("lightSpaceMat", 1, false, glm::value_ptr(light_space_transform));

        // draw table to depth map
        glBindVertexArray(vao_table);
        glDrawArrays(GL_QUADS, table_mtl.vertex_offset, table_mtl.vertex_count);
        glBindVertexArray(0);

        // draw vehicles to shadow map
        glBindVertexArray(vehicle_buffer.get_vao());
        glDrawArraysInstanced(GL_QUADS, vehicle_mtl.vertex_offset, vehicle_mtl.vertex_count, vehicle_buffer.get_num_vehicles());
        glBindVertexArray(0);

        /* DRAW OBJECTS */
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_vis);
        glViewport(0, 0, vis_window_handler.get_width(), vis_window_handler.get_height());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set flags
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);

        // set light values for used shaders
        vis_shader.use();
        set_light_values(vis_shader);
        
        // set view-projection matrix
        vis_shader.uniform_matrix_4x4f("VP", 1, false, glm::value_ptr(glm::mat4(vis_projection * vis_view)));
        vis_shader.uniform_matrix_4x4f("lightSpaceMat", 1, false, glm::value_ptr(light_space_transform));

        /* DRAW TABLE */
        draw_table(vis_shader, table_mtl, vao_table, table_diffuse_map, table_specular_map, shadow_maps);

        /* DRAW VEHICLES */
        draw_vehicles(vis_shader, vehicle_mtl, vehicle_buffer.get_vao(), vehicle_buffer.get_num_vehicles(), vehicle_diffuse_map, vehicle_specular_map, shadow_maps);

       /* -----------------------------------------------------------------------------
        * SCENE (GUI + VISUALIZATION)
        * 
        * Everything gets rendered to a seperate framebuffer.
        * Then the output is rendered as a single texture to a quad to be able to
        * make post-processing effects with the scene (e.g. gamma-correction).
        * -----------------------------------------------------------------------------*/
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_scene);
        glViewport(0, 0, main_window_handler.get_width(), main_window_handler.get_height());
        glClearColor(Main::BACKGROUND_COLOR[0], Main::BACKGROUND_COLOR[1], Main::BACKGROUND_COLOR[2], Main::BACKGROUND_COLOR[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        // set flags
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        /* DRAW GUI */
        draw_GUI(element_shader, text_shader, element_renderer, element_font);
    
        /* DRAW VISUALIZATION SCENE */
        draw_fb(fb_scene_shader, vao_fb, tex_vis, 0);

        /* -----------------------------------------------------------------------------
         * SCREEN (SCENE)
         * -----------------------------------------------------------------------------*/
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // set flags
        glDisable(GL_BLEND);

        /* DRAW SCENE */
        draw_scene(scene_shader, vao_fb, tex_scene, 4);

        // disable remaining active flags
        glDisable(GL_TEXTURE_2D);
        glUseProgram(0);    // unuse shader

        glfwSwapBuffers();
        main_running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

        glEndQuery(GL_TIME_ELAPSED);
        int time;
        glGetQueryObjectiv(timer_query, GL_QUERY_RESULT, &time);
        time_results.push_back(time);

        //std::cout << "MSPF: " << time / 1000000.0 << std::endl;
    }

    glDeleteQueries(1, &timer_query);
    std::cout << get_msg("INFO / OpenGL QUERY") << "All queries deleted." << std::endl;

    // terminating vehicle simulation
    vehicle_processor.stop();
    std::cout << get_msg("INFO / SIMU") << "Stopped simulation processor." << std::endl; 

    // terminating / closing GUI
    std::cout << get_msg("INFO / GUI") << "Terminating GUI..." << std::endl;
    element_renderer.stop();
    std::cout << get_msg("INFO / GUI") << "Stopped Element-Renderer." << std::endl;
    element_handler.stop();
    std::cout << get_msg("INFO / GUI") << "Stopped Element-Handler." << std::endl;
    std::cout << get_msg("INFO / GUI") << "Successfully terminated GUI." << std::endl;

    std::cout << get_msg("INFO / OpenGL") << "Deleting buffers..." << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_fb);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &vbo_fb);
    glDeleteBuffers(2, vbo_table);
    std::cout << get_msg("INFO / OpenGL") << "All vertex buffers deleted." << std::endl;

    glDeleteVertexArrays(1, &vao_fb);
    glDeleteVertexArrays(1, &vao_table);
    std::cout << get_msg("INFO / OpenGL") << "All vertex arrays deleted." << std::endl;

    glDeleteTextures(1, &table_diffuse_map);
    glDeleteTextures(1, &table_specular_map);
    glDeleteTextures(1, &tex_scene);
    glDeleteTextures(1, &tex_vis);
    glDeleteTextures(1, shadow_maps);
    std::cout << get_msg("INFO / OpenGL") << "All textures deleted." << std::endl;

    glDeleteRenderbuffers(1, &rbo_vis);
    std::cout << get_msg("INFO / OpenGL") << "All render buffers deleted." << std::endl;

    glDeleteFramebuffers(1, &fbo_scene);
    glDeleteFramebuffers(1, &fbo_vis);
    glDeleteFramebuffers(1, &fbo_worldlight_shadow);
    std::cout << get_msg("INFO / OpenGL") << "All framebuffers deleted." << std::endl;

    glfwTerminate();
    std::cout << get_msg("INFO / OpenGL WINDOW") << "OpenGL window Closed." << std::endl;
    std::cout << get_msg("INFO / OpenGL") << "OpenGL content successfully terminated." << std::endl;

    try
    {
        Schwarm::Client::shutdown_pathserver(&simu_running, sharedsimumem);
    }
    catch(const std::system_error& e)
    {
        std::cerr << get_msg("ERROR / SERVER") << "Runtime error occured while shutting down server." << std::endl;
    }
    std::cout << get_msg("INFO / SERVER") << "Stopped simulation server." << std::endl;

    event_handler.stop();
    event_handler.cleanup();
    std::cout << get_msg("INFO / EVENT HANDLER") << "Stopped Event-Handler." << std::endl;

    std::cout <<  get_msg("INFO") << "MIN-Frame-Time: " << min_time(time_results) / 1000.0f / 1000.0f << "ms / frame" << std::endl;
    std::cout <<  get_msg("INFO") << "MAX-Frame-Time: " << max_time(time_results) / 1000.0f / 1000.0f << "ms / frame" << std::endl;
    std::cout <<  get_msg("INFO") << "AVG-Frame-Time: " << avg_time(time_results) / 1000.0f / 1000.0f << "ms / frame" << std::endl;

    std::cout << get_msg("INFO / EXIT") << "Exit status 0." << std::endl;
    return 0;   // you have been terminated
}
