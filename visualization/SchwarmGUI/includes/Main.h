#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glfw.h>
#include <GL/glstr.h>
#include <GL/Shader.h>
#include <GL/glc.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <CL/opencl.h>
#include <Event.h>
#include <Model.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include <stb_image.h>

#include "../GUI/gui_includes/gui.h"
#include "../Vehicle/vehicle.h"
#include "../client/client.h"
#include "window_hanlder.h"
#include "my_msg.h"
#include "my_event.h"

#ifndef M_PI
    #define M_PI 3.141592653589793d
#endif

namespace Main
{
    struct Location
    {
        double x;
        double y;
        double z;

        double rotx;
        double roty;
    };

    enum ViewState
    {
        PAUSE,
        CAMERA_VIEW,
        OPEN_WORLD
    };

    /* -----------------------------------------------------------------------------
     * FUNCTIONS REQUIERED FOR THE CONSTANTS
     * -----------------------------------------------------------------------------*/
    float apply_gamma(float f, float __gamma)   {return powf(f, __gamma);}   

    /* -----------------------------------------------------------------------------
     * GLOBAL CONSTANTS
     * -----------------------------------------------------------------------------*/
    float BACKGROUND_COLOR[4];

    float BUTTON_STD_COLOR[16];
    float BUTTON_CLICKED_COLOR[16];
    float BUTTON_TEXT_COLOR[4];

    float TEXTBOX_ACTIVE_COLOR[16];
    float TEXTBOX_INACTIVE_COLOR[16];
    float TEXTBOX_CURSOR_COLOR[4];
    float TEXTBOX_ACTIVE_TEXT_COLOR[4];
    float TEXTBOX_INACTIVE_TEXT_COLOR[4];

    namespace
    {
        constexpr int CHANGE_VIEW   = GLFW_KEY_F5;
        constexpr int PAUSE_KEY     = GLFW_KEY_F1;

        float vis_scene_pos_x = 1.0f, vis_scene_pos_y = 1.0f;
        float vis_scene_width = 1.0f, vis_scene_height = 1.0f;

        float table_size_x = 0.0f;
        float table_size_y = 0.0f;
        float table_origin_x = 0.0f;
        float table_origin_y = 0.0f;

        Location cam_loc            = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
        float fov                   = 0.0f;

        float mouse_sensetivity     = 0.000f;
        float move_speed            = 0.0f;

        float gamma_value           = 2.2f;
        bool auto_gamma_value       = false;

        glm::vec3 worldlight_pos(0.0f);

        ViewState view_state = ViewState::PAUSE;

        void __reload_background_color(void)
        {
            const float temp[4] = {apply_gamma(0.15f, gamma_value), apply_gamma(0.15f, gamma_value), apply_gamma(0.15f, gamma_value), 1.0f};
            memcpy(BACKGROUND_COLOR, temp, sizeof(temp));
        }

        void __reload_button_std_color(void)
        {
            const float temp[16] =  {apply_gamma(0.2f, gamma_value), apply_gamma(0.2f, gamma_value), apply_gamma(0.2f, gamma_value), 1.0f,
                                     apply_gamma(0.2f, gamma_value), apply_gamma(0.2f, gamma_value), apply_gamma(0.2f, gamma_value), 1.0f,
                                     apply_gamma(0.2f, gamma_value), apply_gamma(0.2f, gamma_value), apply_gamma(0.2f, gamma_value), 1.0f,
                                     apply_gamma(0.2f, gamma_value), apply_gamma(0.2f, gamma_value), apply_gamma(0.2f, gamma_value), 1.0f};
            memcpy(BUTTON_STD_COLOR, temp, sizeof(temp));
        }

        void __reload_button_clicked_color(void)
        {
            const float temp[16] =  {apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 1.0f,
                                     apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 1.0f,
                                     apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 1.0f,
                                     apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 1.0f};
            memcpy(BUTTON_CLICKED_COLOR, temp, sizeof(temp));
        }

        void __reload_button_text_color(void)
        {
            const float temp[4] = {apply_gamma(1.0f, gamma_value), apply_gamma(1.0f, gamma_value), apply_gamma(1.0f, gamma_value), 1.0f};
             memcpy(BUTTON_TEXT_COLOR, temp, sizeof(temp));
        }

        void __reload_textbox_active_color(void)
        {
            const float temp[16] =  {apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 1.0f,
                                     apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 1.0f,
                                     apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 1.0f,
                                     apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 1.0f};
            memcpy(TEXTBOX_ACTIVE_COLOR, temp, sizeof(temp));
        }

        void __reload_textbox_inactive_color(void)
        {
            const float temp[16] =  {apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 0.5f,
                                     apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 0.5f,
                                     apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 0.5f,
                                     apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), apply_gamma(0.1f, gamma_value), 0.5f};
            memcpy(TEXTBOX_INACTIVE_COLOR, temp, sizeof(temp));
        }

        void __reload_textbox_cursor_color(void)
        {
            const float temp[4] =  {apply_gamma(1.0f, gamma_value), apply_gamma(1.0f, gamma_value), apply_gamma(1.0f, gamma_value), 1.0f};
            memcpy(TEXTBOX_CURSOR_COLOR, temp, sizeof(temp));
        }

        void __reload_textbox_active_text_color(void)
        {
             const float temp[4] =  {apply_gamma(1.0f, gamma_value), apply_gamma(1.0f, gamma_value), apply_gamma(1.0f, gamma_value), 1.0f};
             memcpy(TEXTBOX_ACTIVE_TEXT_COLOR, temp, sizeof(temp));
        }

        void __reload_textbox_inactive_text_volor(void)
        {
            const float temp[4] =   {apply_gamma(1.0f, gamma_value), apply_gamma(1.0f, gamma_value), apply_gamma(1.0f, gamma_value), 0.5f};
            memcpy(TEXTBOX_INACTIVE_COLOR, temp, sizeof(temp));
        }
    }

    void reload_colors(void)
    {
        __reload_background_color();

        __reload_button_std_color();
        __reload_button_clicked_color();
        __reload_button_text_color();

        __reload_textbox_active_color();
        __reload_textbox_inactive_color();
        __reload_textbox_cursor_color();
        __reload_textbox_active_text_color();
        __reload_textbox_inactive_text_volor();
    }

    /* -----------------------------------------------------------------------------
     * FUNCTIONS FOR VISUALIZATION SCENE
     * -----------------------------------------------------------------------------*/
    void set_vis_pos(float x, float y)
    {
        vis_scene_pos_x = x;
        vis_scene_pos_y = y;
    }
    void set_vis_width(float width)     {vis_scene_width = width;}
    void set_vis_height(float height)   {vis_scene_height = height;}

    float get_vis_pos_x(void)           {return vis_scene_pos_x;}
    float get_vis_pos_y(void)           {return vis_scene_pos_y;}
    float get_vis_width(void)           {return vis_scene_width;}
    float get_vis_height(void)          {return vis_scene_height;}

    /* -----------------------------------------------------------------------------
     * FUNCTIONS FOR TABLE
     * -----------------------------------------------------------------------------*/
    void set_table_size(float x, float y)
    {
        table_size_x = x;
        table_size_y = y;
    }
    void set_table_origin(float x, float y)
    {
        table_origin_x = x;
        table_origin_y = y;
    }
    
    float get_table_size_x(void)        {return table_size_x;}
    float get_table_size_y(void)        {return table_size_y;}
    float get_table_origin_x(void)      {return table_origin_x;}
    float get_table_origin_y(void)      {return table_origin_y;}

    /* -----------------------------------------------------------------------------
     * SET-DATA-FUNCTIONS FOR FRAMEBUFFERS
     * -----------------------------------------------------------------------------*/

    void vis_scene_data(float* map_fb_data, const WindowHandler& vis_window_handler, const GLFWvidmode& vid_mode)
    {
        const float rel_width = (float)vis_window_handler.get_width() / (float)vid_mode.Width;
        const float rel_height = (float)vis_window_handler.get_height() / (float)vid_mode.Height;
        
        float temp_fbd[16] = {vis_scene_pos_x - vis_scene_width, vis_scene_pos_y,                       0.0f, rel_height,
                              vis_scene_pos_x - vis_scene_width, vis_scene_pos_y - vis_scene_height,    0.0f, 0.0f,
                              vis_scene_pos_x, vis_scene_pos_y - vis_scene_height,                      rel_width, 0.0f,
                              vis_scene_pos_x, vis_scene_pos_y,                                         rel_width, rel_height};
        memmove(map_fb_data, temp_fbd, sizeof(float[16]));
    }

    void render_scene_data(float* map_fb_data, const WindowHandler& main_window_handler, const GLFWvidmode& vid_mode)
    {
        const float rel_width = (float)main_window_handler.get_width() / (float)vid_mode.Width;
        const float rel_height = (float)main_window_handler.get_height() / (float)vid_mode.Height;

        float temp_fbd[16] = {-1.0f, 1.0f,  0.0f, rel_height,
                              -1.0f, -1.0f, 0.0f, 0.0f,
                              1.0f, -1.0f,  rel_width, 0.0f,
                              1.0f, 1.0f,   rel_width, rel_height};

        memmove(map_fb_data + 16, temp_fbd, sizeof(float[16]));
    }

    size_t fb_data_size(void)           {return sizeof(float[16]);}

    /* -----------------------------------------------------------------------------
     * MOUSE AND KEYBOARD ACTION FUNCTIONS
     * -----------------------------------------------------------------------------*/
    void mouse_action(int sx, int sy, double& rotx, double& roty, float sensetivity)
    {
        constexpr double roty_max = 89.9/180.0*M_PI;
        const int sx_half = sx / 2;
        const int sy_half = sy / 2;

        int mx, my;
        glfwGetMousePos(&mx, &my);

        const int deltax = sx_half - mx;
        const int deltay = sy_half - my;

        rotx += deltax * sensetivity;
        roty += deltay * sensetivity;

        if(rotx >= 2*M_PI)
            rotx -= 2*M_PI;
        else if(rotx <= -2*M_PI)
            rotx += 2*M_PI;

        if(roty > roty_max)
            roty = roty_max;
        else if(roty < -roty_max)
            roty = -roty_max;

        glfwSetMousePos(sx_half, sy_half);
    }

    void move_action(double& posx, double& posy, double& posz, double rotx, float speed)
    {
        static double old_time = glfwGetTime();
        double deltatime = glfwGetTime() - old_time;
        old_time = glfwGetTime();

        if(glfwGetKey('W') == GLFW_PRESS)
        {
            posx += speed * deltatime * sin(rotx);
            posz += speed * deltatime * cos(rotx);
        }
        if(glfwGetKey('A') == GLFW_PRESS)
        {
            posx += speed * deltatime * sin(rotx + M_PI / 2);
            posz += speed * deltatime * cos(rotx + M_PI / 2);
        }
        if(glfwGetKey('S') == GLFW_PRESS)
        {
            posx += speed * deltatime * sin(rotx + M_PI);
            posz += speed * deltatime * cos(rotx + M_PI);
        }
        if(glfwGetKey('D') == GLFW_PRESS)
        {
            posx += speed * deltatime * sin(rotx + M_PI * 1.5);
            posz += speed * deltatime * cos(rotx + M_PI * 1.5);
        }
        if(glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS)
            posy += speed * deltatime;
        if(glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS)
            posy -= speed * deltatime;
    }

    bool any_key_pressed(void)
    {
        return (glfwGetKey(CHANGE_VIEW)     == GLFW_PRESS ||
                glfwGetKey(PAUSE_KEY)       == GLFW_PRESS);
    }

    void process_key_events()
    {
        static bool key_pressed = false;
        static Location old_location = cam_loc;

        if(!key_pressed && glfwGetKey(CHANGE_VIEW) == GLFW_PRESS)
        {
            if(!key_pressed && view_state == ViewState::OPEN_WORLD)
            {
                glfwEnable(GLFW_MOUSE_CURSOR);
                old_location = cam_loc;
                cam_loc = {0.0, 1.565, 0.0, 0.0, -(M_PI / 2) + 0.001};
                view_state = ViewState::CAMERA_VIEW;
            }
            else if(view_state == ViewState::CAMERA_VIEW)
            {
                glfwDisable(GLFW_MOUSE_CURSOR);
                cam_loc = old_location;
                view_state = ViewState::OPEN_WORLD;
            }
            key_pressed = true;
        }

        if(!key_pressed && glfwGetKey(PAUSE_KEY) == GLFW_PRESS && view_state != ViewState::CAMERA_VIEW)
        {
            if(view_state == ViewState::PAUSE)
            {
                glfwDisable(GLFW_MOUSE_CURSOR);
                view_state = ViewState::OPEN_WORLD;
            }
            else if(view_state == ViewState::OPEN_WORLD)
            {
                glfwEnable(GLFW_MOUSE_CURSOR);
                view_state = ViewState::PAUSE;
            }
            key_pressed = true;
        }

        if(key_pressed && !any_key_pressed())
            key_pressed = false;
    }

    ViewState get_view_state(void)      {return view_state;}
    
    /* -----------------------------------------------------------------------------
     * FUNCTIONS FOR MOVEMENT
     * -----------------------------------------------------------------------------*/
    void set_sensetivity(float f)       {mouse_sensetivity = f;}
    void set_move_speed(float f)        {move_speed = f;}
    float get_sensetivity(void)         {return mouse_sensetivity;}
    float get_move_speed(void)          {return move_speed;}

    /* -----------------------------------------------------------------------------
     * FUNCTIONS FOR CAMERA
     * -----------------------------------------------------------------------------*/
    Location& camera_location(void)     {return cam_loc;}
    void set_fov(float f)               {fov = f;}
    float get_fov(void)                 {return fov;}

    /* -----------------------------------------------------------------------------
     * FUNCTIONS FOR GAMMA
     * -----------------------------------------------------------------------------*/
    void set_gamma(float f)             {gamma_value = f;}
    float get_gamma(void)               {return gamma_value;}
    bool& auto_gamma(void)              {return auto_gamma_value;}

    /* -----------------------------------------------------------------------------
     * FUNCTIONS FOR LIGHT
     * -----------------------------------------------------------------------------*/
    void transform_lightpos(glm::vec3& lightpos, float factor)
    {
        lightpos = glm::normalize(lightpos) * factor;
    }
    glm::vec3& worldlight_position(void)    {return worldlight_pos;}
}

#endif // MAIN_H_INCLUDED
